#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * md.c: the statement markdown subset of docs/problem-format.md, parsed into
 * a flat list of blocks.
 *
 * Blocks are recognized line by line in a single pass. Inline content is
 * parsed the CommonMark way: code spans and links are resolved while
 * scanning left to right, and emphasis is matched afterwards on a list of
 * delimiter runs, with per-kind lower bounds so that unmatched markers are
 * never rescanned. Nothing recurses and every pass is close to linear.
 *
 * Everything a document points to lives in one arena. Scratch buffers are
 * malloc'ed and freed before lc_md_parse returns.
 */

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

#define MD_MAX_DEPTH 16  /* list items nest at most this many levels */
#define MD_MAX_PARENS 32 /* nesting of parentheses inside a link url */
#define NONE ((size_t)-1)

/* What lc_md_doc.priv_ points to. */
typedef struct md_doc_impl {
    lc_md_doc doc;
    lc_arena arena;
} md_doc_impl;

/* Inline nodes, kept in text order. */
enum { N_TEXT, N_CODE, N_DELIM, N_BRACKET, N_BREAK };

typedef struct md_node {
    size_t off;        /* TEXT, CODE: offset in tbuf; DELIM: run length */
    size_t len;        /* TEXT, CODE: byte count; DELIM: characters left */
    size_t prev, next; /* DELIM: neighbours in the delimiter list */
    const char *url;   /* set once the node is known to be inside a link */
    int dstrong, dem;  /* strong/em nesting changes at this node */
    unsigned char type, ch, can_open, can_close, dead;
} md_node;

/* A span being built; its text is at `off` in sbuf. */
typedef struct md_span {
    unsigned flags;
    size_t off, len;
    const char *url;
} md_span;

/* A maximal run of backticks in the inline text. */
typedef struct md_run {
    size_t len, pos;
} md_run;

enum { CUR_NONE, CUR_PARA, CUR_ITEM, CUR_QUOTE };

typedef struct md_parser {
    lc_arena *arena;
    int nomem;

    lc_md_block *blocks;
    size_t nblocks, capblocks;

    /* The open paragraph, list item or quote, and its raw inline text. */
    int cur, cur_line, cur_ordered, cur_number, cur_depth;
    lc_str para;

    /* The open fenced code block. */
    int in_code, code_line;
    char fence_ch;
    size_t fence_len, fence_indent;
    const char *code_lang;
    lc_str code;

    /* List context: marker indentation and numbering for each depth. */
    size_t nlevels;
    size_t level_indent[MD_MAX_DEPTH];
    int num_active[MD_MAX_DEPTH];
    int num_next[MD_MAX_DEPTH];

    /* Inline scratch, reused for every block. */
    md_node *nodes;
    size_t nnodes, capnodes;
    size_t last_delim;  /* tail of the delimiter list */
    size_t *brackets;   /* nodes of unresolved '[' */
    size_t nbrackets, capbrackets;
    size_t link_floor;  /* brackets before this node are inactive */
    md_run *runs;       /* sorted by length, then position */
    size_t nruns, capruns;
    lc_str tbuf;
    md_span *spans;
    size_t nspans, capspans;
    int span_open;      /* the last span may still grow */
    lc_str sbuf;
} md_parser;

/* ------------------------------------------------------------------ */
/* Character classes (ASCII only; the locale never matters)            */
/* ------------------------------------------------------------------ */

static int is_blank(int c)
{
    return c == ' ' || c == '\t';
}

static int is_space(int c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' ||
           c == '\v';
}

static int is_punct(int c)
{
    return (c >= '!' && c <= '/') || (c >= ':' && c <= '@') ||
           (c >= '[' && c <= '`') || (c >= '{' && c <= '~');
}

/* Letters, digits and non-ASCII bytes: '_' can't open or close beside them. */
static int is_word(int c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') || c >= 0x80;
}

static int is_special(int c)
{
    return c == '\\' || c == '\n' || c == '`' || c == '*' || c == '_' ||
           c == '[' || c == ']';
}

#define UC(c) ((unsigned char)(c))

/* ------------------------------------------------------------------ */
/* Inline: building nodes                                              */
/* ------------------------------------------------------------------ */

/* Appends a node. Pointers to earlier nodes are invalid afterwards. */
static md_node *new_node(md_parser *mp, int type)
{
    md_node *nd;

    if (mp->nnodes == mp->capnodes) {
        md_node *tmp = lc_grow(mp->nodes, &mp->capnodes, mp->nnodes + 1,
                               sizeof *tmp);
        if (!tmp) {
            mp->nomem = 1;
            return NULL;
        }
        mp->nodes = tmp;
    }
    nd = &mp->nodes[mp->nnodes++];
    memset(nd, 0, sizeof *nd);
    nd->type = (unsigned char)type;
    nd->prev = nd->next = NONE;
    return nd;
}

/* Literal text, appended to the last node when that is plain text. */
static void add_text(md_parser *mp, const char *s, size_t n)
{
    md_node *last = mp->nnodes ? &mp->nodes[mp->nnodes - 1] : NULL;

    if (n == 0)
        return;
    if (!last || last->type != N_TEXT || last->url) {
        last = new_node(mp, N_TEXT);
        if (!last)
            return;
        last->off = mp->tbuf.len;
    }
    if (lc_str_append(&mp->tbuf, s, n) != LC_OK) {
        mp->nomem = 1;
        return;
    }
    last->len += n;
}

/* Drops spaces and tabs at the end of the text just added (before a break). */
static void trim_text(md_parser *mp)
{
    md_node *last;

    if (mp->nnodes == 0)
        return;
    last = &mp->nodes[mp->nnodes - 1];
    if (last->type != N_TEXT || last->url)
        return;
    while (last->len > 0 &&
           is_blank(UC(mp->tbuf.data[last->off + last->len - 1]))) {
        last->len--;
        mp->tbuf.len--;
    }
    if (mp->tbuf.data)
        mp->tbuf.data[mp->tbuf.len] = '\0';
}

static void add_break(md_parser *mp)
{
    trim_text(mp);
    new_node(mp, N_BREAK);
}

static int cmp_run(const void *a, const void *b)
{
    const md_run *x = a, *y = b;

    if (x->len != y->len)
        return x->len < y->len ? -1 : 1;
    if (x->pos != y->pos)
        return x->pos < y->pos ? -1 : 1;
    return 0;
}

/* Records every maximal backtick run, so closers are found by binary search. */
static void collect_runs(md_parser *mp, const char *s, size_t n)
{
    size_t i = 0;

    mp->nruns = 0;
    while (i < n) {
        const char *p = memchr(s + i, '`', n - i);
        size_t j;

        if (!p)
            break;
        i = (size_t)(p - s);
        for (j = i; j < n && s[j] == '`'; j++)
            ;
        if (mp->nruns == mp->capruns) {
            md_run *tmp = lc_grow(mp->runs, &mp->capruns, mp->nruns + 1,
                                  sizeof *tmp);
            if (!tmp) {
                mp->nomem = 1;
                return;
            }
            mp->runs = tmp;
        }
        mp->runs[mp->nruns].len = j - i;
        mp->runs[mp->nruns].pos = i;
        mp->nruns++;
        i = j;
    }
    if (mp->nruns > 1)
        qsort(mp->runs, mp->nruns, sizeof *mp->runs, cmp_run);
}

/* Position of the first backtick run of exactly `len` at or after `from`. */
static size_t find_run(const md_parser *mp, size_t len, size_t from)
{
    size_t lo = 0, hi = mp->nruns;

    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        const md_run *r = &mp->runs[mid];

        if (r->len < len || (r->len == len && r->pos < from))
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < mp->nruns && mp->runs[lo].len == len)
        return mp->runs[lo].pos;
    return NONE;
}

/* s[i] is '`'. Returns the index after the code span or the literal run. */
static size_t code_span(md_parser *mp, const char *s, size_t n, size_t i)
{
    size_t j, q, a, b, k;
    md_node *nd;

    for (j = i; j < n && s[j] == '`'; j++)
        ;
    q = find_run(mp, j - i, j);
    if (q == NONE) {
        add_text(mp, s + i, j - i);
        return j;
    }
    /* Line breaks become spaces; then one space goes from each end when
     * both ends have one and the content is not all spaces. */
    a = j;
    b = q;
    if (b - a >= 2 && (s[a] == ' ' || s[a] == '\n') &&
        (s[b - 1] == ' ' || s[b - 1] == '\n')) {
        for (k = a; k < b && (s[k] == ' ' || s[k] == '\n'); k++)
            ;
        if (k < b) {
            a++;
            b--;
        }
    }
    nd = new_node(mp, N_CODE);
    if (!nd)
        return n;
    nd->off = mp->tbuf.len;
    nd->len = b - a;
    if (lc_str_append(&mp->tbuf, s + a, b - a) != LC_OK) {
        mp->nomem = 1;
        return n;
    }
    for (k = nd->off; k < mp->tbuf.len; k++)
        if (mp->tbuf.data[k] == '\n')
            mp->tbuf.data[k] = ' ';
    return q + (j - i);
}

/* s[i] is '*' or '_'. */
static size_t delim_run(md_parser *mp, const char *s, size_t n, size_t i)
{
    int c = UC(s[i]), before, after, can_open, can_close;
    size_t j, idx;
    md_node *nd;

    for (j = i; j < n && UC(s[j]) == c; j++)
        ;
    before = i > 0 ? UC(s[i - 1]) : '\n';
    after = j < n ? UC(s[j]) : '\n';
    can_open = !is_space(after);
    can_close = !is_space(before);
    if (c == '_') {
        can_open = can_open && !is_word(before);
        can_close = can_close && !is_word(after);
    }
    if (!can_open && !can_close) {
        add_text(mp, s + i, j - i);
        return j;
    }
    nd = new_node(mp, N_DELIM);
    if (!nd)
        return n;
    idx = mp->nnodes - 1;
    nd->ch = (unsigned char)c;
    nd->off = nd->len = j - i;
    nd->can_open = (unsigned char)can_open;
    nd->can_close = (unsigned char)can_close;
    nd->prev = mp->last_delim;
    if (mp->last_delim != NONE)
        mp->nodes[mp->last_delim].next = idx;
    mp->last_delim = idx;
    return j;
}

static void remove_delim(md_parser *mp, size_t d)
{
    md_node *nd = &mp->nodes[d];

    if (nd->prev != NONE)
        mp->nodes[nd->prev].next = nd->next;
    if (nd->next != NONE)
        mp->nodes[nd->next].prev = nd->prev;
    else
        mp->last_delim = nd->prev;
    nd->prev = nd->next = NONE;
}

/*
 * Matches emphasis among the delimiters at node `bottom` or later, then
 * drops them from the delimiter list (CommonMark's "process emphasis").
 * openers_bottom[char][closer can open][run length % 3] records, for each
 * kind of closer, below which node a search already failed.
 */
static void process_emphasis(md_parser *mp, size_t bottom)
{
    size_t openers_bottom[2][2][3];
    size_t closer = NONE, d = mp->last_delim;
    md_node *nodes = mp->nodes;
    int x, y, z;

    for (x = 0; x < 2; x++)
        for (y = 0; y < 2; y++)
            for (z = 0; z < 3; z++)
                openers_bottom[x][y][z] = bottom;
    while (d != NONE && d >= bottom) {
        closer = d;
        d = nodes[d].prev;
    }
    while (closer != NONE) {
        md_node *c = &nodes[closer], *o = NULL;
        size_t opener, *ob;

        if (!c->can_close) {
            closer = c->next;
            continue;
        }
        ob = &openers_bottom[c->ch == '_'][c->can_open][c->off % 3];
        for (opener = c->prev; opener != NONE && opener >= *ob;
             opener = nodes[opener].prev) {
            o = &nodes[opener];
            /* The "rule of 3": *foo**bar* is not <em>foo</em>... */
            if (o->can_open && o->ch == c->ch &&
                (!(c->can_open || o->can_close) || c->off % 3 == 0 ||
                 (o->off + c->off) % 3 != 0))
                break;
        }
        if (opener != NONE && opener >= *ob) {
            size_t use = c->len >= 2 && o->len >= 2 ? 2 : 1, k;

            o->len -= use;
            c->len -= use;
            if (use == 2) {
                nodes[opener + 1].dstrong++;
                c->dstrong--;
            } else {
                nodes[opener + 1].dem++;
                c->dem--;
            }
            for (k = c->prev; k != opener;) {
                size_t pk = nodes[k].prev;
                remove_delim(mp, k);
                k = pk;
            }
            if (o->len == 0)
                remove_delim(mp, opener);
            if (c->len == 0) {
                size_t next = c->next;
                remove_delim(mp, closer);
                closer = next;
            }
        } else {
            size_t old = closer;

            closer = c->next;
            *ob = old;
            if (!c->can_open)
                remove_delim(mp, old);
        }
    }
    while (mp->last_delim != NONE && mp->last_delim >= bottom)
        remove_delim(mp, mp->last_delim);
}

/*
 * Parses "(url)" starting at s[i], just after the ']'. On success stores
 * the unescaped url in the arena and the index after ')' in *endp.
 */
static int link_dest(md_parser *mp, const char *s, size_t n, size_t i,
                     size_t *endp, const char **urlp)
{
    size_t a, b, k, depth = 0;
    char *url, *w;

    while (i < n && is_space(UC(s[i])))
        i++;
    a = i;
    while (i < n) {
        int c = UC(s[i]);

        if (c == '\\' && i + 1 < n && is_punct(UC(s[i + 1]))) {
            i += 2;
            continue;
        }
        if (is_space(c))
            break;
        if (c == '(') {
            if (++depth > MD_MAX_PARENS)
                return 0;
        } else if (c == ')') {
            if (depth == 0)
                break;
            depth--;
        }
        i++;
    }
    b = i;
    if (depth != 0)
        return 0;
    while (i < n && is_space(UC(s[i])))
        i++;
    if (i >= n || s[i] != ')')
        return 0;
    url = lc_arena_alloc(mp->arena, b - a + 1);
    if (!url) {
        mp->nomem = 1;
        return 0;
    }
    for (w = url, k = a; k < b; k++) {
        if (s[k] == '\\' && k + 1 < b && is_punct(UC(s[k + 1])))
            k++;
        *w++ = s[k];
    }
    *w = '\0';
    *endp = i + 1;
    *urlp = url;
    return 1;
}

/* s[i] is ']'. */
static size_t close_bracket(md_parser *mp, const char *s, size_t n, size_t i)
{
    size_t b, end, k;
    const char *url;

    if (mp->nbrackets == 0) {
        add_text(mp, "]", 1);
        return i + 1;
    }
    b = mp->brackets[--mp->nbrackets];
    if (b >= mp->link_floor && i + 1 < n && s[i + 1] == '(' &&
        link_dest(mp, s, n, i + 2, &end, &url)) {
        process_emphasis(mp, b);
        mp->nodes[b].dead = 1;
        for (k = b + 1; k < mp->nnodes; k++)
            mp->nodes[k].url = url;
        /* No links inside links: every earlier '[' is now inactive. */
        mp->link_floor = mp->nnodes;
        return end;
    }
    add_text(mp, "]", 1);
    return i + 1;
}

static void open_bracket(md_parser *mp)
{
    if (!new_node(mp, N_BRACKET))
        return;
    if (mp->nbrackets == mp->capbrackets) {
        size_t *tmp = lc_grow(mp->brackets, &mp->capbrackets,
                              mp->nbrackets + 1, sizeof *tmp);
        if (!tmp) {
            mp->nomem = 1;
            return;
        }
        mp->brackets = tmp;
    }
    mp->brackets[mp->nbrackets++] = mp->nnodes - 1;
}

/* ------------------------------------------------------------------ */
/* Inline: emitting spans                                              */
/* ------------------------------------------------------------------ */

static int same_url(const char *a, const char *b)
{
    if (!a || !b)
        return a == b;
    return a == b || strcmp(a, b) == 0;
}

static void span_end(md_parser *mp)
{
    if (mp->span_open) {
        lc_str_putc(&mp->sbuf, '\0');
        mp->span_open = 0;
    }
}

static void span_put(md_parser *mp, unsigned flags, const char *url,
                     const char *s, size_t n)
{
    md_span *sp;

    if (n == 0)
        return;
    if (mp->span_open) {
        sp = &mp->spans[mp->nspans - 1];
        if (sp->flags == flags && same_url(sp->url, url)) {
            lc_str_append(&mp->sbuf, s, n);
            sp->len += n;
            return;
        }
        span_end(mp);
    }
    if (mp->nspans == mp->capspans) {
        md_span *tmp = lc_grow(mp->spans, &mp->capspans, mp->nspans + 1,
                               sizeof *tmp);
        if (!tmp) {
            mp->nomem = 1;
            return;
        }
        mp->spans = tmp;
    }
    sp = &mp->spans[mp->nspans++];
    sp->flags = flags;
    sp->url = url;
    sp->off = mp->sbuf.len;
    sp->len = n;
    lc_str_append(&mp->sbuf, s, n);
    mp->span_open = 1;
}

static void span_break(md_parser *mp)
{
    md_span *sp;

    span_end(mp);
    if (mp->nspans == mp->capspans) {
        md_span *tmp = lc_grow(mp->spans, &mp->capspans, mp->nspans + 1,
                               sizeof *tmp);
        if (!tmp) {
            mp->nomem = 1;
            return;
        }
        mp->spans = tmp;
    }
    sp = &mp->spans[mp->nspans++];
    sp->flags = LC_MD_SPAN_BREAK;
    sp->url = NULL;
    sp->off = mp->sbuf.len;
    sp->len = 0;
    lc_str_putc(&mp->sbuf, '\0');
}

static void emit_spans(md_parser *mp, const lc_md_span **spansp,
                       size_t *nspansp)
{
    static const char stars[] = "********************************";
    static const char unders[] = "________________________________";
    int strong = 0, em = 0;
    size_t k, first, last;
    char *text;
    md_span *sp;
    lc_md_span *out;

    mp->nspans = 0;
    mp->span_open = 0;
    lc_str_clear(&mp->sbuf);
    for (k = 0; k < mp->nnodes && !mp->nomem; k++) {
        const md_node *nd = &mp->nodes[k];
        unsigned flags = 0;

        strong += nd->dstrong;
        em += nd->dem;
        if (strong > 0)
            flags |= LC_MD_SPAN_STRONG;
        if (em > 0)
            flags |= LC_MD_SPAN_EM;
        if (nd->url)
            flags |= LC_MD_SPAN_LINK;
        switch (nd->type) {
        case N_TEXT:
            span_put(mp, flags, nd->url, mp->tbuf.data + nd->off, nd->len);
            break;
        case N_CODE:
            flags &= ~(unsigned)(LC_MD_SPAN_STRONG | LC_MD_SPAN_EM);
            span_put(mp, flags | LC_MD_SPAN_CODE, nd->url,
                     mp->tbuf.data + nd->off, nd->len);
            break;
        case N_DELIM: {
            const char *src = nd->ch == '*' ? stars : unders;
            size_t left = nd->len;

            while (left > 0) {
                size_t chunk = left < 32 ? left : 32;

                span_put(mp, flags, nd->url, src, chunk);
                left -= chunk;
            }
            break;
        }
        case N_BRACKET:
            if (!nd->dead)
                span_put(mp, flags, nd->url, "[", 1);
            break;
        default:
            span_break(mp);
            break;
        }
    }
    span_end(mp);
    if (mp->sbuf.failed)
        mp->nomem = 1;
    if (mp->nomem)
        return;

    /* Breaks and blanks at either end go (code text stays verbatim). */
    for (first = 0, last = mp->nspans;; first++) {
        while (first < last && mp->spans[first].flags == LC_MD_SPAN_BREAK)
            first++;
        if (first == last || (mp->spans[first].flags & LC_MD_SPAN_CODE))
            break;
        sp = &mp->spans[first];
        while (sp->len > 0 && is_blank(UC(mp->sbuf.data[sp->off]))) {
            sp->off++;
            sp->len--;
        }
        if (sp->len > 0)
            break;
    }
    for (; last > first; last--) {
        while (last > first && mp->spans[last - 1].flags == LC_MD_SPAN_BREAK)
            last--;
        if (last == first || (mp->spans[last - 1].flags & LC_MD_SPAN_CODE))
            break;
        sp = &mp->spans[last - 1];
        while (sp->len > 0 &&
               is_blank(UC(mp->sbuf.data[sp->off + sp->len - 1])))
            mp->sbuf.data[sp->off + --sp->len] = '\0';
        if (sp->len > 0)
            break;
    }
    if (first == last)
        return;

    text = lc_arena_alloc(mp->arena, mp->sbuf.len);
    out = lc_arena_array(mp->arena, last - first, sizeof *out);
    if (!text || !out) {
        mp->nomem = 1;
        return;
    }
    memcpy(text, mp->sbuf.data, mp->sbuf.len);
    for (k = first; k < last; k++) {
        out[k - first].flags = mp->spans[k].flags;
        out[k - first].text = text + mp->spans[k].off;
        out[k - first].len = mp->spans[k].len;
        out[k - first].url = mp->spans[k].url;
    }
    *spansp = out;
    *nspansp = last - first;
}

/*
 * Parses inline text: lines joined with '\n', the first line's leading and
 * the last line's trailing blanks already removed.
 */
static void md_inline(md_parser *mp, const char *s, size_t n,
                      const lc_md_span **spansp, size_t *nspansp)
{
    size_t i = 0, j;

    *spansp = NULL;
    *nspansp = 0;
    mp->nnodes = 0;
    mp->nbrackets = 0;
    mp->last_delim = NONE;
    mp->link_floor = 0;
    mp->nruns = 0;
    lc_str_clear(&mp->tbuf);
    if (n > 0 && memchr(s, '`', n))
        collect_runs(mp, s, n);

    while (i < n && !mp->nomem) {
        switch (s[i]) {
        case '\\':
            if (i + 1 < n && s[i + 1] == '\n') {
                add_break(mp);
                i += 2;
            } else if (i + 1 < n && is_punct(UC(s[i + 1]))) {
                add_text(mp, s + i + 1, 1);
                i += 2;
            } else {
                add_text(mp, "\\", 1);
                i++;
            }
            break;
        case '\n':
            if (i >= 2 && s[i - 1] == ' ' && s[i - 2] == ' ') {
                add_break(mp);
            } else {
                trim_text(mp);
                add_text(mp, " ", 1);
            }
            for (i++; i < n && is_blank(UC(s[i])); i++)
                ;
            break;
        case '`':
            i = code_span(mp, s, n, i);
            break;
        case '*':
        case '_':
            i = delim_run(mp, s, n, i);
            break;
        case '[':
            open_bracket(mp);
            i++;
            break;
        case ']':
            i = close_bracket(mp, s, n, i);
            break;
        default:
            for (j = i + 1; j < n && !is_special(UC(s[j])); j++)
                ;
            add_text(mp, s + i, j - i);
            i = j;
            break;
        }
    }
    if (mp->nomem)
        return;
    process_emphasis(mp, 0);
    emit_spans(mp, spansp, nspansp);
}

/* ------------------------------------------------------------------ */
/* Blocks                                                              */
/* ------------------------------------------------------------------ */

static lc_md_block *push_block(md_parser *mp, lc_md_kind kind, int line)
{
    lc_md_block *b;

    if (mp->nomem)
        return NULL;
    if (mp->nblocks == mp->capblocks) {
        lc_md_block *tmp = lc_grow(mp->blocks, &mp->capblocks,
                                   mp->nblocks + 1, sizeof *tmp);
        if (!tmp) {
            mp->nomem = 1;
            return NULL;
        }
        mp->blocks = tmp;
    }
    b = &mp->blocks[mp->nblocks++];
    memset(b, 0, sizeof *b);
    b->kind = kind;
    b->line = line;
    return b;
}

/* Pushes a block with inline content parsed from s[0..n). */
static void push_inline(md_parser *mp, lc_md_kind kind, int line,
                        const char *s, size_t n, lc_md_block *proto)
{
    const lc_md_span *spans;
    size_t nspans;
    lc_md_block *b;

    md_inline(mp, s, n, &spans, &nspans);
    b = push_block(mp, kind, line);
    if (!b)
        return;
    if (proto) {
        b->level = proto->level;
        b->ordered = proto->ordered;
        b->number = proto->number;
        b->depth = proto->depth;
    }
    b->spans = spans;
    b->nspans = nspans;
}

/* Closes the open paragraph, list item or quote, if any. */
static void close_cur(md_parser *mp)
{
    lc_md_block proto;
    lc_md_kind kind;

    if (mp->cur == CUR_NONE)
        return;
    if (mp->para.failed)
        mp->nomem = 1;
    while (mp->para.len > 0 && is_blank(UC(mp->para.data[mp->para.len - 1])))
        mp->para.len--;
    memset(&proto, 0, sizeof proto);
    if (mp->cur == CUR_ITEM) {
        kind = LC_MD_LIST_ITEM;
        proto.ordered = mp->cur_ordered;
        proto.number = mp->cur_number;
        proto.depth = mp->cur_depth;
    } else {
        kind = mp->cur == CUR_QUOTE ? LC_MD_QUOTE : LC_MD_PARAGRAPH;
    }
    if (!mp->nomem)
        push_inline(mp, kind, mp->cur_line, lc_str_cstr(&mp->para),
                    mp->para.len, &proto);
    mp->cur = CUR_NONE;
    lc_str_clear(&mp->para);
}

static void start_cur(md_parser *mp, int cur, int line, const char *s, size_t n)
{
    mp->cur = cur;
    mp->cur_line = line;
    lc_str_append(&mp->para, s, n);
}

/* A continuation line of the open block. */
static void continue_cur(md_parser *mp, const char *s, size_t n)
{
    if (mp->para.len > 0)
        lc_str_putc(&mp->para, '\n');
    lc_str_append(&mp->para, s, n);
}

/* Any block other than a list item ends every list. */
static void reset_lists(md_parser *mp)
{
    mp->nlevels = 0;
    memset(mp->num_active, 0, sizeof mp->num_active);
}

/* A block that is not a paragraph, item or quote: ends all of those. */
static void leaf_start(md_parser *mp)
{
    close_cur(mp);
    reset_lists(mp);
}

static void close_code(md_parser *mp)
{
    lc_md_block *b;
    char *text;

    mp->in_code = 0;
    if (mp->code.failed) {
        mp->nomem = 1;
        return;
    }
    text = lc_arena_strndup(mp->arena, lc_str_cstr(&mp->code), mp->code.len);
    if (!text) {
        mp->nomem = 1;
        return;
    }
    b = push_block(mp, LC_MD_CODE, mp->code_line);
    if (b) {
        b->lang = mp->code_lang;
        b->text = text;
        b->len = mp->code.len;
    }
    lc_str_clear(&mp->code);
}

/* A line inside a fenced code block. */
static void code_line(md_parser *mp, const char *s, size_t n)
{
    size_t i = 0, k;

    while (i < n && i < 4 && s[i] == ' ')
        i++;
    if (i <= 3 && i < n && s[i] == mp->fence_ch) {
        for (k = i; k < n && s[k] == mp->fence_ch; k++)
            ;
        if (k - i >= mp->fence_len) {
            while (k < n && is_blank(UC(s[k])))
                k++;
            if (k == n) {
                close_code(mp);
                return;
            }
        }
    }
    for (i = 0; i < n && i < mp->fence_indent && s[i] == ' '; i++)
        ;
    lc_str_append(&mp->code, s + i, n - i);
    lc_str_putc(&mp->code, '\n');
}

/* s is a line's text after `indent` (<= 3) spaces. */
static int fence_open(md_parser *mp, const char *s, size_t n, size_t indent,
                      int line)
{
    char c = s[0];
    size_t k, a, w;
    char *lang;

    if (c != '`' && c != '~')
        return 0;
    for (k = 0; k < n && s[k] == c; k++)
        ;
    if (k < 3)
        return 0;
    /* As in CommonMark: ```foo``` on one line is a code span, not a fence. */
    if (c == '`' && k < n && memchr(s + k, '`', n - k))
        return 0;
    for (a = k; a < n && is_blank(UC(s[a])); a++)
        ;
    for (w = a; w < n && !is_blank(UC(s[w])); w++)
        ;
    leaf_start(mp);
    lang = lc_arena_strndup(mp->arena, s + a, w - a);
    if (!lang) {
        mp->nomem = 1;
        return 1;
    }
    mp->in_code = 1;
    mp->fence_ch = c;
    mp->fence_len = k;
    mp->fence_indent = indent;
    mp->code_lang = lang;
    mp->code_line = line;
    lc_str_clear(&mp->code);
    return 1;
}

static int is_rule(const char *s, size_t n)
{
    char c = s[0];
    size_t i, count = 0;

    if (c != '-' && c != '*' && c != '_')
        return 0;
    for (i = 0; i < n; i++) {
        if (s[i] == c)
            count++;
        else if (!is_blank(UC(s[i])))
            return 0;
    }
    return count >= 3;
}

static int heading(md_parser *mp, const char *s, size_t n, int line)
{
    size_t k, a, b, h;
    lc_md_block proto;

    for (k = 0; k < n && s[k] == '#'; k++)
        ;
    if (k == 0 || k > 6 || k == n || !is_blank(UC(s[k])))
        return 0;
    for (a = k; a < n && is_blank(UC(s[a])); a++)
        ;
    for (b = n; b > a && is_blank(UC(s[b - 1])); b--)
        ;
    /* A closing run of '#' preceded by a blank goes. */
    for (h = b; h > a && s[h - 1] == '#'; h--)
        ;
    if (h < b && (h == a || is_blank(UC(s[h - 1])))) {
        for (b = h; b > a && is_blank(UC(s[b - 1])); b--)
            ;
    }
    leaf_start(mp);
    memset(&proto, 0, sizeof proto);
    proto.level = (int)k;
    push_inline(mp, LC_MD_HEADING, line, s + a, b - a, &proto);
    return 1;
}

/* s[0] is '>'. */
static void quote_line(md_parser *mp, const char *s, size_t n, int line)
{
    size_t i;

    for (i = 1; i < n && is_blank(UC(s[i])); i++)
        ;
    if (i == n) { /* an empty quote line separates quote paragraphs */
        leaf_start(mp);
        return;
    }
    if (mp->cur == CUR_QUOTE) {
        continue_cur(mp, s + i, n - i);
        return;
    }
    leaf_start(mp);
    start_cur(mp, CUR_QUOTE, line, s + i, n - i);
}

/*
 * Recognizes "- ", "* ", "+ " and "1. " / "1) " (one to nine digits).
 * On success sets *ordered, *number and *text (index of the item's text).
 */
static int list_marker(const char *s, size_t n, int *ordered, int *number,
                       size_t *text)
{
    size_t i = 0;

    if (s[0] == '-' || s[0] == '*' || s[0] == '+') {
        *ordered = 0;
        *number = 0;
        i = 1;
    } else {
        int v = 0;

        for (; i < n && s[i] >= '0' && s[i] <= '9'; i++) {
            if (i == 9)
                return 0;
            v = v * 10 + (s[i] - '0');
        }
        if (i == 0 || i >= n || (s[i] != '.' && s[i] != ')'))
            return 0;
        *ordered = 1;
        *number = v;
        i++;
    }
    if (i >= n || !is_blank(UC(s[i])))
        return 0;
    while (i < n && is_blank(UC(s[i])))
        i++;
    *text = i;
    return 1;
}

static void list_item(md_parser *mp, size_t indent, int ordered, int number,
                      int line, const char *s, size_t n)
{
    size_t depth = 0, k;

    /* Two or more columns deeper than the enclosing item's marker nests. */
    if (mp->nlevels > 0) {
        k = mp->nlevels;
        while (k > 0 && indent < mp->level_indent[k - 1] + 2)
            k--;
        depth = k < MD_MAX_DEPTH ? k : MD_MAX_DEPTH - 1;
    }
    mp->level_indent[depth] = indent;
    mp->nlevels = depth + 1;
    for (k = depth + 1; k < MD_MAX_DEPTH; k++)
        mp->num_active[k] = 0;
    if (!ordered) {
        mp->num_active[depth] = 0;
    } else if (mp->num_active[depth]) {
        number = mp->num_next[depth];
        mp->num_next[depth] = number < INT_MAX ? number + 1 : INT_MAX;
    } else {
        mp->num_active[depth] = 1;
        mp->num_next[depth] = number + 1;
    }
    mp->cur_ordered = ordered;
    mp->cur_number = number;
    mp->cur_depth = (int)depth;
    start_cur(mp, CUR_ITEM, line, s, n);
}

static int is_examples(const char *s, size_t n)
{
    while (n > 0 && is_blank(UC(s[n - 1])))
        n--;
    return n == 9 && memcmp(s, "@examples", 9) == 0;
}

static void md_line(md_parser *mp, const char *s, size_t n, int line)
{
    size_t i = 0, col = 0, text;
    int ordered, number;

    if (mp->in_code) {
        code_line(mp, s, n);
        return;
    }
    for (; i < n && is_blank(UC(s[i])); i++)
        col = s[i] == '\t' ? col + 4 - col % 4 : col + 1;
    if (i == n) {
        close_cur(mp); /* lists survive blank lines */
        return;
    }
    s += i;
    n -= i;

    if (is_examples(s, n)) {
        leaf_start(mp);
        push_block(mp, LC_MD_EXAMPLES, line);
        return;
    }
    if (col <= 3) {
        if (fence_open(mp, s, n, i, line))
            return;
        if (is_rule(s, n)) {
            leaf_start(mp);
            push_block(mp, LC_MD_RULE, line);
            return;
        }
        if (heading(mp, s, n, line))
            return;
        if (s[0] == '>') {
            quote_line(mp, s, n, line);
            return;
        }
    }
    /* Only a numbered item starting at 1 may interrupt a paragraph. */
    if (list_marker(s, n, &ordered, &number, &text) &&
        !(mp->cur == CUR_PARA && ordered && number != 1)) {
        close_cur(mp);
        list_item(mp, col, ordered, number, line, s + text, n - text);
        return;
    }
    if (mp->cur == CUR_PARA || mp->cur == CUR_ITEM) {
        continue_cur(mp, s, n);
        return;
    }
    leaf_start(mp);
    start_cur(mp, CUR_PARA, line, s, n);
}

/* ------------------------------------------------------------------ */
/* Public API                                                          */
/* ------------------------------------------------------------------ */

/* Copies the text with each NUL byte replaced by U+FFFD. */
static char *replace_nuls(const char *text, size_t len, size_t *outlen)
{
    size_t i, nuls = 0, n;
    char *buf, *w;

    for (i = 0; i < len; i++)
        if (text[i] == '\0')
            nuls++;
    if (nuls > (SIZE_MAX - len) / 2)
        return NULL;
    n = len + 2 * nuls;
    buf = malloc(n);
    if (!buf)
        return NULL;
    for (w = buf, i = 0; i < len; i++) {
        if (text[i] == '\0') {
            *w++ = '\xEF';
            *w++ = '\xBF';
            *w++ = '\xBD';
        } else {
            *w++ = text[i];
        }
    }
    *outlen = n;
    return buf;
}

static void parser_free(md_parser *mp)
{
    lc_str_free(&mp->para);
    lc_str_free(&mp->code);
    lc_str_free(&mp->tbuf);
    lc_str_free(&mp->sbuf);
    free(mp->blocks);
    free(mp->nodes);
    free(mp->brackets);
    free(mp->runs);
    free(mp->spans);
}

int lc_md_parse(const char *text, size_t len, lc_md_doc **out)
{
    md_doc_impl *impl;
    md_parser mp;
    char *norm = NULL;
    const char *p, *end;
    lc_md_block *blocks = NULL;
    int line = 0, lone_cr = 0;

    if (!out)
        return LC_ERR_INVALID;
    *out = NULL;
    if (!text) {
        text = "";
        len = 0;
    }
    impl = calloc(1, sizeof *impl);
    if (!impl)
        return LC_ERR_NOMEM;
    memset(&mp, 0, sizeof mp);
    mp.arena = &impl->arena;
    mp.last_delim = NONE;

    if (len > 0 && memchr(text, '\0', len)) {
        norm = replace_nuls(text, len, &len);
        if (norm) {
            text = norm;
        } else {
            mp.nomem = 1;
            len = 0;
        }
    }
    p = text;
    end = text + len;
    /* Lines end at "\n", "\r\n" or a lone "\r", as in CommonMark. Line
     * numbers count "\n" only, like the loader and every editor: a line
     * after a lone "\r" keeps the number of the line it is part of, so
     * statement_line + line - 1 is always the line in problem.md. */
    while (p < end && !mp.nomem) {
        const char *e = p;

        while (e < end && *e != '\n' && *e != '\r')
            e++;
        if (!lone_cr && line < INT_MAX)
            line++;
        md_line(&mp, p, (size_t)(e - p), line);
        lone_cr = 0;
        if (e < end && *e == '\r') {
            if (e + 1 < end && e[1] == '\n')
                e++;
            else
                lone_cr = 1;
        }
        p = e < end ? e + 1 : e;
    }
    if (!mp.nomem) {
        if (mp.in_code)
            close_code(&mp);
        else
            close_cur(&mp);
    }
    if (!mp.nomem && mp.nblocks > 0) {
        blocks = lc_arena_array(&impl->arena, mp.nblocks, sizeof *blocks);
        if (blocks)
            memcpy(blocks, mp.blocks, mp.nblocks * sizeof *blocks);
        else
            mp.nomem = 1;
    }
    free(norm);
    parser_free(&mp);
    if (mp.nomem) {
        lc_arena_free(&impl->arena);
        free(impl);
        return LC_ERR_NOMEM;
    }
    impl->doc.blocks = blocks;
    impl->doc.nblocks = mp.nblocks;
    impl->doc.priv_ = impl;
    *out = &impl->doc;
    return LC_OK;
}

void lc_md_free(lc_md_doc *doc)
{
    md_doc_impl *impl;

    if (!doc)
        return;
    impl = doc->priv_;
    lc_arena_free(&impl->arena);
    free(impl);
}
