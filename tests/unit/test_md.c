#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"
#include "test.h"

/*
 * Allocation failures. The parser and the arena.c and str.c code it uses
 * are compiled into this file, with malloc, calloc, realloc and free routed
 * through the wrappers below, so build this file on its own (or link it
 * with liblc.a, which then contributes nothing), not together with those
 * three .c files. The wrappers count allocations and, while oom_parse()
 * runs, can make one of them fail. Every header those files include is
 * included above, before the macros are defined.
 */
static struct {
    int armed;    /* count, and maybe fail, the allocations made now */
    long count;   /* allocations attempted while armed */
    long fail_at; /* the one that fails (counting from 0), or -1 */
    int sticky;   /* and so does every one after it */
    long live;    /* blocks allocated and not yet freed */
} oom = {0, 0, -1, 0, 0};

static int oom_fail(void)
{
    long k;

    if (!oom.armed)
        return 0;
    k = oom.count++;
    return oom.fail_at >= 0 &&
           (k == oom.fail_at || (oom.sticky && k > oom.fail_at));
}

static void *oom_malloc(size_t n)
{
    void *p = oom_fail() ? NULL : malloc(n);

    if (p)
        oom.live++;
    return p;
}

static void *oom_calloc(size_t count, size_t size)
{
    void *p = oom_fail() ? NULL : calloc(count, size);

    if (p)
        oom.live++;
    return p;
}

static void *oom_realloc(void *p, size_t n)
{
    void *q = oom_fail() ? NULL : realloc(p, n);

    if (q && !p)
        oom.live++;
    return q;
}

static void oom_free(void *p)
{
    if (p)
        oom.live--;
    free(p);
}

#define malloc(n) oom_malloc(n)
#define calloc(count, size) oom_calloc(count, size)
#define realloc(p, n) oom_realloc(p, n)
#define free(p) oom_free(p)
#include "arena.c"
#include "str.c"
#include "md.c"
#undef malloc
#undef calloc
#undef realloc
#undef free

/*
 * Documents are rendered to a compact text form, one block per line:
 *
 *     P@1: text        paragraph on line 1
 *     H2@1: text       heading, level 2
 *     C@1[lang]: a\nb\n  code block (newlines shown as \n)
 *     L0.3@1: text     numbered list item, depth 0, number 3
 *     L1-@1: text      bulleted list item, depth 1
 *     Q@1: text        quote
 *     R@1              thematic break
 *     X@1              @examples placeholder
 *
 * Spans render as <a url>, <b>, <i>, <c> around their text (in that order)
 * and a hard break as <br>. Spans are merged when their flags match, so tag
 * boundaries show exactly where spans start and end.
 */

static void render_span(lc_str *o, const lc_md_span *sp)
{
    if (sp->flags & LC_MD_SPAN_BREAK) {
        lc_str_puts(o, "<br>");
        return;
    }
    if (sp->flags & LC_MD_SPAN_LINK)
        lc_str_printf(o, "<a %s>", sp->url ? sp->url : "NULL");
    if (sp->flags & LC_MD_SPAN_STRONG)
        lc_str_puts(o, "<b>");
    if (sp->flags & LC_MD_SPAN_EM)
        lc_str_puts(o, "<i>");
    if (sp->flags & LC_MD_SPAN_CODE)
        lc_str_puts(o, "<c>");
    lc_str_append(o, sp->text, sp->len);
    if (sp->flags & LC_MD_SPAN_CODE)
        lc_str_puts(o, "</c>");
    if (sp->flags & LC_MD_SPAN_EM)
        lc_str_puts(o, "</i>");
    if (sp->flags & LC_MD_SPAN_STRONG)
        lc_str_puts(o, "</b>");
    if (sp->flags & LC_MD_SPAN_LINK)
        lc_str_puts(o, "</a>");
}

static void render_block(lc_str *o, const lc_md_block *b)
{
    size_t i;

    switch (b->kind) {
    case LC_MD_PARAGRAPH:
        lc_str_printf(o, "P@%d:", b->line);
        break;
    case LC_MD_HEADING:
        lc_str_printf(o, "H%d@%d:", b->level, b->line);
        break;
    case LC_MD_CODE:
        lc_str_printf(o, "C@%d[%s]: ", b->line, b->lang);
        for (i = 0; i < b->len; i++) {
            if (b->text[i] == '\n')
                lc_str_puts(o, "\\n");
            else
                lc_str_putc(o, b->text[i]);
        }
        lc_str_putc(o, '\n');
        return;
    case LC_MD_LIST_ITEM:
        if (b->ordered)
            lc_str_printf(o, "L%d.%d@%d:", b->depth, b->number, b->line);
        else
            lc_str_printf(o, "L%d-@%d:", b->depth, b->line);
        break;
    case LC_MD_QUOTE:
        lc_str_printf(o, "Q@%d:", b->line);
        break;
    case LC_MD_RULE:
        lc_str_printf(o, "R@%d\n", b->line);
        return;
    case LC_MD_EXAMPLES:
        lc_str_printf(o, "X@%d\n", b->line);
        return;
    default:
        lc_str_puts(o, "?\n");
        return;
    }
    lc_str_putc(o, ' ');
    for (i = 0; i < b->nspans; i++)
        render_span(o, &b->spans[i]);
    lc_str_putc(o, '\n');
}

/* The result is malloc'ed, or NULL when out of memory. */
static char *render_doc(const lc_md_doc *doc)
{
    lc_str o = LC_STR_INIT;
    char *s;
    size_t i;

    for (i = 0; i < doc->nblocks; i++)
        render_block(&o, &doc->blocks[i]);
    s = o.failed ? NULL : strdup(lc_str_cstr(&o));
    lc_str_free(&o);
    return s;
}

/* Number of lines, counted the way the parser does ("\n", "\r\n", "\r"). */
static size_t count_lines(const char *s, size_t n)
{
    size_t i = 0, lines = 0;

    while (i < n) {
        lines++;
        while (i < n && s[i] != '\n' && s[i] != '\r')
            i++;
        if (i < n && s[i] == '\r' && i + 1 < n && s[i + 1] == '\n')
            i++;
        if (i < n)
            i++;
    }
    return lines;
}

static void show_source(const char *src, size_t len)
{
    size_t i;

    fprintf(stderr, "    source (%lu bytes): \"", (unsigned long)len);
    for (i = 0; i < len && i < 200; i++) {
        unsigned char c = (unsigned char)src[i];
        if (c >= 32 && c < 127 && c != '\\' && c != '"')
            fputc(c, stderr);
        else
            fprintf(stderr, "\\x%02x", c);
    }
    fprintf(stderr, "%s\"\n", len > 200 ? "..." : "");
}

#define INV(cond)                                                         \
    do {                                                                  \
        if (!(cond)) {                                                    \
            fprintf(stderr, "%s:%d: invariant failed: %s (block %lu)\n",  \
                    __FILE__, __LINE__, #cond, (unsigned long)bi);        \
            show_source(src, len);                                        \
            return 0;                                                     \
        }                                                                 \
    } while (0)

/* Checks properties every parsed document must have. */
static int doc_ok(const lc_md_doc *doc, const char *src, size_t len)
{
    size_t bi = 0, si, lines = count_lines(src, len);
    int prev_line = 1;

    INV(doc != NULL);
    INV(doc->nblocks == 0 || doc->blocks != NULL);
    for (bi = 0; bi < doc->nblocks; bi++) {
        const lc_md_block *b = &doc->blocks[bi];
        int has_spans = b->kind == LC_MD_PARAGRAPH ||
                        b->kind == LC_MD_HEADING ||
                        b->kind == LC_MD_LIST_ITEM || b->kind == LC_MD_QUOTE;

        INV(b->kind >= LC_MD_PARAGRAPH && b->kind <= LC_MD_EXAMPLES);
        INV(b->line >= prev_line);
        INV(b->line >= 1 && (size_t)b->line <= lines);
        prev_line = b->line;
        INV(b->nspans == 0 || b->spans != NULL);
        if (!has_spans)
            INV(b->nspans == 0);
        if (b->kind == LC_MD_HEADING)
            INV(b->level >= 1 && b->level <= 6);
        else
            INV(b->level == 0);
        if (b->kind == LC_MD_LIST_ITEM) {
            INV(b->depth >= 0 && b->depth < 16);
            INV(b->ordered ? b->number >= 0 : b->number == 0);
        } else {
            INV(b->ordered == 0 && b->number == 0 && b->depth == 0);
        }
        if (b->kind == LC_MD_CODE) {
            INV(b->lang != NULL && b->text != NULL);
            INV(strlen(b->text) == b->len);
            INV(b->len == 0 || b->text[b->len - 1] == '\n');
            INV(strchr(b->lang, ' ') == NULL && strchr(b->lang, '\t') == NULL);
        } else {
            INV(b->lang == NULL && b->text == NULL && b->len == 0);
        }
        for (si = 0; si < b->nspans; si++) {
            const lc_md_span *sp = &b->spans[si];

            INV(sp->text != NULL);
            INV(strlen(sp->text) == sp->len);
            INV((sp->flags & ~31u) == 0);
            INV(!(sp->flags & LC_MD_SPAN_CODE) ||
                !(sp->flags & (LC_MD_SPAN_STRONG | LC_MD_SPAN_EM)));
            INV(!(sp->flags & LC_MD_SPAN_LINK) == !sp->url);
            if (sp->flags & LC_MD_SPAN_BREAK) {
                INV(sp->flags == LC_MD_SPAN_BREAK && sp->len == 0);
                INV(si > 0 && si + 1 < b->nspans);
            } else {
                INV(sp->len > 0);
            }
            if (si > 0 && !(sp->flags & LC_MD_SPAN_BREAK)) {
                const lc_md_span *pv = &b->spans[si - 1];
                INV(pv->flags != sp->flags ||
                    (pv->url && sp->url && strcmp(pv->url, sp->url) != 0));
            }
        }
        if (b->nspans > 0) {
            const lc_md_span *f = &b->spans[0], *l = &b->spans[b->nspans - 1];
            INV((f->text[0] != ' ' && f->text[0] != '\t') ||
                (f->flags & LC_MD_SPAN_CODE));
            INV((l->text[l->len - 1] != ' ' && l->text[l->len - 1] != '\t') ||
                (l->flags & LC_MD_SPAN_CODE));
        }
    }
    return 1;
}

/* lc_md_parse with allocation `fail_at` failing (-1: none). */
static int oom_parse(const char *src, size_t len, long fail_at, int sticky,
                     lc_md_doc **doc)
{
    int rc;

    oom.armed = 1;
    oom.count = 0;
    oom.fail_at = fail_at;
    oom.sticky = sticky;
    rc = lc_md_parse(src, len, doc);
    oom.armed = 0;
    return rc;
}

/*
 * Parses src again with each allocation failing in turn, first on its own
 * and then with every later one failing too. Each run must fail cleanly
 * (LC_ERR_NOMEM, *out NULL, nothing left allocated) or give exactly `want`,
 * the rendering of a normal parse: never a partial document.
 */
static int oom_ok(const char *src, size_t len, const char *want)
{
    static lc_md_doc sentinel;
    lc_md_doc *doc = NULL;
    long live = oom.live, nallocs, k;
    int sticky;

    if (oom_parse(src, len, -1, 0, &doc) != LC_OK || !doc)
        return 0;
    lc_md_free(doc);
    nallocs = oom.count;
    for (sticky = 0; sticky < 2; sticky++) {
        for (k = 0; k < nallocs; k++) {
            const char *why = NULL;
            char *got = NULL;
            int rc;

            doc = &sentinel;
            rc = oom_parse(src, len, k, sticky, &doc);
            if (rc == LC_OK) {
                got = doc ? render_doc(doc) : NULL;
                lc_md_free(doc);
                if (!got || strcmp(got, want) != 0)
                    why = "a different document";
            } else if (rc != LC_ERR_NOMEM) {
                why = "a status other than LC_ERR_NOMEM";
            } else if (doc != NULL) {
                why = "LC_ERR_NOMEM with *out not NULL";
            }
            if (!why && oom.live != live)
                why = "a leak";
            if (why) {
                fprintf(stderr, "%s:%d: allocation %ld of %ld failing%s "
                        "gave %s\n", __FILE__, __LINE__, k + 1, nallocs,
                        sticky ? " (and all later ones)" : "", why);
                if (got)
                    fprintf(stderr, "    got:\n%s    expected:\n%s", got,
                            want);
                show_source(src, len);
                free(got);
                return 0;
            }
            free(got);
        }
    }
    return 1;
}

/* A malloc'ed copy with no terminator, so a read past `len` is caught. */
static char *copy_exact(const char *src, size_t len)
{
    char *p = malloc(len ? len : 1);

    if (p && len)
        memcpy(p, src, len);
    return p;
}

/*
 * Parses src[0..len) from an exact-size copy, checks the document and the
 * parser's handling of allocation failures, and returns the rendering
 * (malloc'ed).
 */
static char *render(const char *src, size_t len)
{
    lc_md_doc *doc = NULL;
    char *copy = copy_exact(src, len), *out;
    int rc;

    CHECK(copy != NULL);
    if (!copy)
        return strdup("<error>");
    rc = lc_md_parse(copy, len, &doc);
    CHECK_INT(rc, LC_OK);
    if (rc != LC_OK || !doc) {
        free(copy);
        return strdup("<error>");
    }
    CHECK(doc_ok(doc, copy, len));
    out = render_doc(doc);
    lc_md_free(doc);
    CHECK(out != NULL);
    if (out)
        CHECK(oom_ok(copy, len, out));
    free(copy);
    return out ? out : strdup("<error>");
}

#define MDN(src, n, expect)                                               \
    do {                                                                  \
        char *got_ = render((src), (n));                                  \
        CHECK_STR(got_, (expect));                                        \
        free(got_);                                                       \
    } while (0)

#define MD(src, expect) MDN((src), strlen(src), (expect))

/* ------------------------------------------------------------------ */

static void test_empty(void)
{
    lc_md_doc *doc = NULL;

    MD("", "");
    MD("   ", "");
    MD("\n\n\n", "");
    MD(" \t \n\t\n  \r\n \r", "");
    CHECK_INT(lc_md_parse(NULL, 0, &doc), LC_OK);
    CHECK(doc != NULL && doc->nblocks == 0 && doc->blocks == NULL);
    lc_md_free(doc);
    lc_md_free(NULL);
}

static void test_paragraphs(void)
{
    MD("hello", "P@1: hello\n");
    MD("a\nb\nc", "P@1: a b c\n");
    MD("a\n\nb", "P@1: a\nP@3: b\n");
    MD("a\n \t \nb", "P@1: a\nP@3: b\n");
    MD("  a \n   b  ", "P@1: a b\n");
    MD("a\t\t\nb", "P@1: a b\n");
    MD("    indented four", "P@1: indented four\n");
    MD("x\n        y", "P@1: x y\n");
    MD("tab\tinside", "P@1: tab\tinside\n");
    MD("< > & caf\xc3\xa9 \xe2\x89\xa4",
       "P@1: < > & caf\xc3\xa9 \xe2\x89\xa4\n");
}

static void test_hard_breaks(void)
{
    MD("a  \nb", "P@1: a<br>b\n");
    MD("a     \nb", "P@1: a<br>b\n");
    MD("a\\\nb", "P@1: a<br>b\n");
    MD("a \\\nb", "P@1: a<br>b\n");
    MD("a \nb", "P@1: a b\n");
    MD("a\\\\\nb", "P@1: a\\ b\n");
    MD("a\\  \nb", "P@1: a\\<br>b\n");
    /* At the end of a block a break is dropped; a final '\' is literal. */
    MD("a  ", "P@1: a\n");
    MD("a  \n\nb", "P@1: a\nP@3: b\n");
    MD("a\\", "P@1: a\\\n");
    MD("a\\\n\\", "P@1: a<br>\\\n");
    /* A break before any text is dropped too. */
    MD("\\\nb", "P@1: b\n");
    MD("a\\\n\\\nb", "P@1: a<br><br>b\n");
    MD("*a  \nb*", "P@1: <i>a</i><br><i>b</i>\n");
    MD("- a  \n  b", "L0-@1: a<br>b\n");
    MD("> a  \n> b", "Q@1: a<br>b\n");
    /* Headings are one line: trailing spaces and '\' are not breaks. */
    MD("# a\\", "H1@1: a\\\n");
}

static void test_headings(void)
{
    MD("# Title", "H1@1: Title\n");
    MD("## Two", "H2@1: Two\n");
    MD("###### six", "H6@1: six\n");
    MD("####### seven", "P@1: ####### seven\n");
    MD("#foo", "P@1: #foo\n");
    MD("#", "P@1: #\n");
    MD("# ", "H1@1: \n");
    MD("#\tTab", "H1@1: Tab\n");
    MD("#   spaced   ", "H1@1: spaced\n");
    MD("   # three", "H1@1: three\n");
    MD("    # four", "P@1: # four\n");
    MD("# foo ##", "H1@1: foo\n");
    MD("# foo ##   ", "H1@1: foo\n");
    MD("# foo#", "H1@1: foo#\n");
    MD("# foo \\##", "H1@1: foo ##\n");
    MD("# #", "H1@1: \n");
    MD("## ###", "H2@1: \n");
    MD("# a # b", "H1@1: a # b\n");
    MD("# a *b* `c`", "H1@1: a <i>b</i> <c>c</c>\n");
    MD("# a\nb", "H1@1: a\nP@2: b\n");
    MD("para\n# H\nmore", "P@1: para\nH1@2: H\nP@3: more\n");
}

static void test_code_blocks(void)
{
    MD("```\ncode\n```", "C@1[]: code\\n\n");
    MD("```cpp extra words\nint x;\n\n  y\n```\nafter",
       "C@1[cpp]: int x;\\n\\n  y\\n\nP@6: after\n");
    MD("```  text  \nx\n```", "C@1[text]: x\\n\n");
    MD("```\n```", "C@1[]: \n");
    MD("~~~\na\n~~~~", "C@1[]: a\\n\n");
    MD("~~~~\na\n~~~\n", "C@1[]: a\\n~~~\\n\n");
    MD("```\na\n~~~\n```", "C@1[]: a\\n~~~\\n\n");
    MD("````\na\n```\n````", "C@1[]: a\\n```\\n\n");
    MD("```\na\n```   \nb", "C@1[]: a\\n\nP@4: b\n");
    MD("```\na\n``` x\n```", "C@1[]: a\\n``` x\\n\n");
    MD("```\na\n   ```", "C@1[]: a\\n\n");
    MD("```\na\n    ```", "C@1[]: a\\n    ```\\n\n");
    /* Unclosed: runs to the end, blank lines included. */
    MD("```py\nx = 1\n", "C@1[py]: x = 1\\n\n");
    MD("```py\nx = 1\n\n\n", "C@1[py]: x = 1\\n\\n\\n\n");
    MD("```", "C@1[]: \n");
    MD("text\n```\n# not a heading\n- not an item\n@examples\n",
       "P@1: text\nC@2[]: # not a heading\\n- not an item\\n@examples\\n\n");
    /* Indented fences strip that much indentation from content lines. */
    MD("  ```\n    a\n  b\n c\n\td\n  ```", "C@1[]:   a\\nb\\nc\\n\td\\n\n");
    MD("   ~~~\n      x\n   ~~~", "C@1[]:    x\\n\n");
    MD("    ```\nx", "P@1: ``` x\n");
    MD("\t```\nx", "P@1: ``` x\n");
    /* Tabs and trailing spaces inside are verbatim. */
    MD("```\n\tx  \n```", "C@1[]: \tx  \\n\n");
    /* A backtick fence's info string can't contain a backtick. */
    MD("```a`b\nfoo", "P@1: ```a`b foo\n");
    MD("``` x ```", "P@1: <c>x</c>\n");
    MD("~~~ a`b\nx\n~~~", "C@1[a`b]: x\\n\n");
    MD("``\nx", "P@1: `` x\n");
    /* Fences interrupt paragraphs and list items. */
    MD("para\n```\ncode\n```", "P@1: para\nC@2[]: code\\n\n");
    MD("- item\n```\ncode\n```", "L0-@1: item\nC@2[]: code\\n\n");
}

static void test_lists(void)
{
    MD("- a\n- b", "L0-@1: a\nL0-@2: b\n");
    MD("* a\n+ b\n- c", "L0-@1: a\nL0-@2: b\nL0-@3: c\n");
    MD("-\tfoo", "L0-@1: foo\n");
    MD("-   foo", "L0-@1: foo\n");
    MD("- ", "L0-@1: \n");
    MD("- \nfoo", "L0-@1: foo\n");
    MD("-a", "P@1: -a\n");
    MD("-", "P@1: -\n");
    MD("1.a", "P@1: 1.a\n");
    MD("1.", "P@1: 1.\n");
    MD("- **bold** and `code`", "L0-@1: <b>bold</b> and <c>code</c>\n");
    /* The text after a marker is inline text, never another block. */
    MD("- - x", "L0-@1: - x\n");
    MD("- # x", "L0-@1: # x\n");
    MD("- > x", "L0-@1: > x\n");
    /* Continuation lines. */
    MD("- a\nb\n    c", "L0-@1: a b c\n");
    MD("1. a\n2 * 3", "L0.1@1: a 2 * 3\n");
    /* A blank line ends the item; what follows is a new block. */
    MD("- a\n\n  b", "L0-@1: a\nP@3: b\n");
    MD("- a\n\n    b\n- c", "L0-@1: a\nP@3: b\nL0-@4: c\n");
    /* Other blocks end items. */
    MD("- a\n# h", "L0-@1: a\nH1@2: h\n");
    MD("- a\n> q", "L0-@1: a\nQ@2: q\n");
    MD("- a\n---", "L0-@1: a\nR@2\n");
    MD("- a\n@examples", "L0-@1: a\nX@2\n");
    MD("- a\n2. b", "L0-@1: a\nL0.2@2: b\n");
    /* Thematic breaks win over bullets. */
    MD("- - -", "R@1\n");
    MD("* * *", "R@1\n");
}

static void test_list_nesting(void)
{
    MD("- a\n  - b\n    - c\n  - d\n- e",
       "L0-@1: a\nL1-@2: b\nL2-@3: c\nL1-@4: d\nL0-@5: e\n");
    MD("- a\n - b", "L0-@1: a\nL0-@2: b\n");
    MD("- a\n   - b\n  - c", "L0-@1: a\nL1-@2: b\nL1-@3: c\n");
    /* Siblings compare with the latest marker at their depth. */
    MD("- a\n   - b\n    - c\n     - d\n       - e",
       "L0-@1: a\nL1-@2: b\nL1-@3: c\nL1-@4: d\nL2-@5: e\n");
    MD("    - a\n- b", "L0-@1: a\nL0-@2: b\n");
    MD("  - a\n- b\n  - c", "L0-@1: a\nL0-@2: b\nL1-@3: c\n");
    MD("- a\n\t- b", "L0-@1: a\nL1-@2: b\n");
    MD("- a\n\n  - b", "L0-@1: a\nL1-@3: b\n");
    /* A paragraph ends the list: the next marker starts at depth 0. */
    MD("- a\n\npara\n  - b", "L0-@1: a\nP@3: para\nL0-@4: b\n");
    MD("para\n  - b\n    - c", "P@1: para\nL0-@2: b\nL1-@3: c\n");
    /* Depth stops at 15. */
    {
        lc_str src = LC_STR_INIT, want = LC_STR_INIT;
        int i;

        for (i = 0; i < 20; i++) {
            lc_str_printf(&src, "%*s- x%d\n", 2 * i, "", i);
            lc_str_printf(&want, "L%d-@%d: x%d\n", i < 15 ? i : 15, i + 1, i);
        }
        lc_str_puts(&src, "                              - y\n");
        lc_str_puts(&want, "L15-@21: y\n");
        lc_str_puts(&src, "                            - z\n");
        lc_str_puts(&want, "L14-@22: z\n");
        MD(lc_str_cstr(&src), lc_str_cstr(&want));
        lc_str_free(&src);
        lc_str_free(&want);
    }
}

static void test_list_numbering(void)
{
    MD("1. a\n2. b\n3. c", "L0.1@1: a\nL0.2@2: b\nL0.3@3: c\n");
    MD("3. a\n7. b", "L0.3@1: a\nL0.4@2: b\n");
    MD("1. a\n1. b\n1. c", "L0.1@1: a\nL0.2@2: b\nL0.3@3: c\n");
    MD("1) a\n2) b", "L0.1@1: a\nL0.2@2: b\n");
    MD("0. a\n0. b", "L0.0@1: a\nL0.1@2: b\n");
    MD("007. a", "L0.7@1: a\n");
    MD("123456789. x", "L0.123456789@1: x\n");
    MD("1234567890. x", "P@1: 1234567890. x\n");
    /* Blank lines and deeper items don't break a list. */
    MD("5. a\n\n9. b", "L0.5@1: a\nL0.6@3: b\n");
    MD("1. a\n   - x\n   - y\n1. b",
       "L0.1@1: a\nL1-@2: x\nL1-@3: y\nL0.2@4: b\n");
    MD("1. a\n\n   - x\n\n1. b", "L0.1@1: a\nL1-@3: x\nL0.2@5: b\n");
    /* A shallower item ends the deeper list. */
    MD("1. a\n   1. x\n   2. y\n2. b\n   1. z\n   5. w",
       "L0.1@1: a\nL1.1@2: x\nL1.2@3: y\nL0.2@4: b\nL1.1@5: z\nL1.2@6: w\n");
    MD("1. a\n   4. x\n- b\n   4. y",
       "L0.1@1: a\nL1.4@2: x\nL0-@3: b\nL1.4@4: y\n");
    /* A bullet at the same depth ends a numbered list. */
    MD("1. a\n- b\n1. c", "L0.1@1: a\nL0-@2: b\nL0.1@3: c\n");
    /* So does any other block. */
    MD("1. a\n\npara\n\n4. b", "L0.1@1: a\nP@3: para\nL0.4@5: b\n");
    MD("1. a\n# h\n1. b", "L0.1@1: a\nH1@2: h\nL0.1@3: b\n");
    MD("1. a\n```\nx\n```\n3. b", "L0.1@1: a\nC@2[]: x\\n\nL0.3@5: b\n");
}

static void test_interruptions(void)
{
    MD("text\n# h", "P@1: text\nH1@2: h\n");
    MD("text\n```\nx\n```", "P@1: text\nC@2[]: x\\n\n");
    MD("text\n~~~\nx", "P@1: text\nC@2[]: x\\n\n");
    MD("text\n---", "P@1: text\nR@2\n");
    MD("text\n***", "P@1: text\nR@2\n");
    MD("text\n> q", "P@1: text\nQ@2: q\n");
    MD("text\n- b", "P@1: text\nL0-@2: b\n");
    MD("text\n* b", "P@1: text\nL0-@2: b\n");
    MD("text\n+ b", "P@1: text\nL0-@2: b\n");
    MD("text\n1. one", "P@1: text\nL0.1@2: one\n");
    MD("text\n1) one", "P@1: text\nL0.1@2: one\n");
    MD("text\n01. one", "P@1: text\nL0.1@2: one\n");
    MD("text\n@examples", "P@1: text\nX@2\n");
    /* A numbered item with any other number continues the paragraph. */
    MD("text\n2. more", "P@1: text 2. more\n");
    MD("text\n10) more\n3. x", "P@1: text 10) more 3. x\n");
    MD("text\n0. zero", "P@1: text 0. zero\n");
    /* Not blocks: these continue the paragraph. */
    MD("text\n#nospace", "P@1: text #nospace\n");
    MD("text\n    # indented", "P@1: text # indented\n");
    MD("text\n--", "P@1: text --\n");
    MD("text\n-x", "P@1: text -x\n");
    MD("text\n=====", "P@1: text =====\n");
    MD("text\n@examples here", "P@1: text @examples here\n");
}

static void test_quotes(void)
{
    MD("> a\n> b", "Q@1: a b\n");
    MD(">a\n>b", "Q@1: a b\n");
    MD(">    a", "Q@1: a\n");
    MD("   > a", "Q@1: a\n");
    MD("    > a", "P@1: > a\n");
    MD("> a\n>\n> b", "Q@1: a\nQ@3: b\n");
    MD("> a\n\n> b", "Q@1: a\nQ@3: b\n");
    MD("> a\nb", "Q@1: a\nP@2: b\n");
    MD(">", "");
    MD("> \n>\t", "");
    MD("para\n>\nmore", "P@1: para\nP@3: more\n");
    MD("> # not heading\n> - x\n> > y", "Q@1: # not heading - x > y\n");
    MD("> **b** `c`", "Q@1: <b>b</b> <c>c</c>\n");
    MD("> a\n# h", "Q@1: a\nH1@2: h\n");
    MD("> a\n- x", "Q@1: a\nL0-@2: x\n");
    MD("> a\n2. x", "Q@1: a\nL0.2@2: x\n");
    MD("> ```\n> x", "Q@1: ``` x\n");
}

static void test_rules(void)
{
    MD("---", "R@1\n");
    MD("***", "R@1\n");
    MD("___", "R@1\n");
    MD("- - -", "R@1\n");
    MD(" *  *  * ", "R@1\n");
    MD("_ _ _ _ _", "R@1\n");
    MD("---------------", "R@1\n");
    MD("   ---", "R@1\n");
    MD("    ---", "P@1: ---\n");
    MD("--", "P@1: --\n");
    MD("-*-", "P@1: -*-\n");
    MD("---a", "P@1: ---a\n");
    MD("a\n---\nb", "P@1: a\nR@2\nP@3: b\n");
}

static void test_examples_placeholder(void)
{
    MD("@examples", "X@1\n");
    MD("  @examples  ", "X@1\n");
    MD("\t@examples\t", "X@1\n");
    MD("        @examples", "X@1\n");
    MD("@examples x", "P@1: @examples x\n");
    MD("@example", "P@1: @example\n");
    MD("@Examples", "P@1: @Examples\n");
    MD("para\n@examples\nmore", "P@1: para\nX@2\nP@3: more\n");
    MD("1. a\n@examples\n2. b", "L0.1@1: a\nX@2\nL0.2@3: b\n");
    MD("```\n@examples\n```", "C@1[]: @examples\\n\n");
    MD("> @examples", "Q@1: @examples\n");
    MD("`@examples`", "P@1: <c>@examples</c>\n");
}

static void test_emphasis(void)
{
    MD("*em* and _em_", "P@1: <i>em</i> and <i>em</i>\n");
    MD("**strong** and __strong__", "P@1: <b>strong</b> and <b>strong</b>\n");
    MD("***both***", "P@1: <b><i>both</i></b>\n");
    MD("*a **b** c*", "P@1: <i>a </i><b><i>b</i></b><i> c</i>\n");
    MD("**a *b* c**", "P@1: <b>a </b><b><i>b</i></b><b> c</b>\n");
    MD("**a _b_ c**", "P@1: <b>a </b><b><i>b</i></b><b> c</b>\n");
    MD("*foo*bar", "P@1: <i>foo</i>bar\n");
    MD("*(x)*", "P@1: <i>(x)</i>\n");
    MD("**foo*", "P@1: *<i>foo</i>\n");
    MD("*foo**", "P@1: <i>foo</i>*\n");
    MD("*foo**bar**baz*", "P@1: <i>foo</i><b><i>bar</i></b><i>baz</i>\n");
    MD("*a**b*", "P@1: <i>a**b</i>\n");
    MD("*a\nb*", "P@1: <i>a b</i>\n");
    /* Spaces: an opener needs a non-space after, a closer one before. */
    MD("2 * 3 * 4", "P@1: 2 * 3 * 4\n");
    MD("x * a *", "P@1: x * a *\n");
    MD("x *a *", "P@1: x *a *\n");
    MD("x * a*", "P@1: x * a*\n");
    MD("a ** b ** c", "P@1: a ** b ** c\n");
    MD("x _ a _", "P@1: x _ a _\n");
    /* '_' doesn't open or close inside a word. */
    MD("snake_case_name", "P@1: snake_case_name\n");
    MD("call snake_case_name() now", "P@1: call snake_case_name() now\n");
    MD("_foo_bar", "P@1: _foo_bar\n");
    MD("foo_bar_", "P@1: foo_bar_\n");
    MD("__init__", "P@1: <b>init</b>\n");
    MD("x__init__y", "P@1: x__init__y\n");
    MD("caf\xc3\xa9_x_", "P@1: caf\xc3\xa9_x_\n");
    MD("*in*side", "P@1: <i>in</i>side\n");
    MD("a*b*c", "P@1: a<i>b</i>c\n");
    MD("(_x_)", "P@1: (<i>x</i>)\n");
    /* Unmatched markers are text. */
    MD("*foo", "P@1: *foo\n");
    MD("foo*", "P@1: foo*\n");
    MD("**foo", "P@1: **foo\n");
    MD("*a _b* c_", "P@1: <i>a _b</i> c_\n");
    MD("_a *b_ c*", "P@1: <i>a *b</i> c*\n");
    MD("*a\n\nb*", "P@1: *a\nP@3: b*\n");
    /* Emphasis around a code span: code is never combined with it. */
    MD("**`x`**", "P@1: <c>x</c>\n");
    MD("*a `*` b*", "P@1: <i>a </i><c>*</c><i> b</i>\n");
}

static void test_code_spans(void)
{
    MD("`code`", "P@1: <c>code</c>\n");
    MD("a `b` c", "P@1: a <c>b</c> c\n");
    MD("`` a`b ``", "P@1: <c>a`b</c>\n");
    MD("``a``b`", "P@1: <c>a</c>b`\n");
    MD("`foo``bar`", "P@1: <c>foo``bar</c>\n");
    MD("```a``b```", "P@1: <c>a``b</c>\n");
    MD("`a", "P@1: `a\n");
    MD("``a`", "P@1: ``a`\n");
    MD("a``", "P@1: a``\n");
    /* One space comes off each end only when both have one. */
    MD("` a `", "P@1: <c>a</c>\n");
    MD("`  a  `", "P@1: <c> a </c>\n");
    MD("` a`", "P@1: <c> a</c>\n");
    MD("`a `", "P@1: <c>a </c>\n");
    MD("x ` ` y", "P@1: x <c> </c> y\n");
    MD("x `  ` y", "P@1: x <c>  </c> y\n");
    MD("`` ` ``", "P@1: <c>`</c>\n");
    /* Contents are verbatim; line breaks become spaces. */
    MD("`*a* _b_ [c](d) \\*`", "P@1: <c>*a* _b_ [c](d) \\*</c>\n");
    MD("`a\\`b`", "P@1: <c>a\\</c>b`\n");
    MD("`a\nb`", "P@1: <c>a b</c>\n");
    MD("`a  \nb`", "P@1: <c>a   b</c>\n");
    MD("`\tx\t`", "P@1: <c>\tx\t</c>\n");
    /* A code span wins over emphasis and links. */
    MD("*a`*`", "P@1: *a<c>*</c>\n");
    MD("[a`](b)`", "P@1: [a<c>](b)</c>\n");
    /* An escaped backtick doesn't open, and the rest of the run is shorter. */
    MD("\\`a`", "P@1: `a`\n");
    MD("\\``a`", "P@1: `<c>a</c>\n");
}

static void test_escapes(void)
{
    MD("\\*not em\\*", "P@1: *not em*\n");
    MD("\\_x\\_", "P@1: _x_\n");
    MD("\\\\", "P@1: \\\n");
    MD("\\a\\1\\ ", "P@1: \\a\\1\\\n");
    MD("\\`code\\`", "P@1: `code`\n");
    MD("\\[x](y)", "P@1: [x](y)\n");
    MD("[x\\](y)", "P@1: [x](y)\n");
    MD("\\# not heading", "P@1: # not heading\n");
    MD("\\- not item", "P@1: - not item\n");
    MD("\\@examples", "P@1: @examples\n");
    MD("\\!\\\"\\#\\$\\%\\&\\'\\(\\)\\*\\+\\,\\-\\.\\/\\:\\;\\<\\=\\>\\?\\@"
       "\\[\\\\\\]\\^\\_\\`\\{\\|\\}\\~",
       "P@1: !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~\n");
    MD("\\\xc3\xa9", "P@1: \\\xc3\xa9\n");
    MD("**a\\**", "P@1: *<i>a*</i>\n");
}

static void test_links(void)
{
    MD("[text](http://x.y/z)", "P@1: <a http://x.y/z>text</a>\n");
    MD("see [a](u) and [b](v).",
       "P@1: see <a u>a</a> and <a v>b</a>.\n");
    MD("[**b** and `c`](u)",
       "P@1: <a u><b>b</b></a><a u> and </a><a u><c>c</c></a>\n");
    MD("[*a* b](u)", "P@1: <a u><i>a</i></a><a u> b</a>\n");
    MD("**[a](u)**", "P@1: <a u><b>a</b></a>\n");
    MD("*x [a](u) y*", "P@1: <i>x </i><a u><i>a</i></a><i> y</i>\n");
    MD("[a]( u )", "P@1: <a u>a</a>\n");
    MD("[a]()", "P@1: <a >a</a>\n");
    MD("[a](w_(b))", "P@1: <a w_(b)>a</a>\n");
    MD("[a](x\\)y)", "P@1: <a x)y>a</a>\n");
    MD("[a](x\\y)", "P@1: <a x\\y>a</a>\n");
    MD("[a](<u>)", "P@1: <a <u>>a</a>\n");
    MD("[a\nb](u)", "P@1: <a u>a b</a>\n");
    MD("[a](\nu\n)", "P@1: <a u>a</a>\n");
    /* Not links. */
    MD("[a](b c)", "P@1: [a](b c)\n");
    MD("[a](u \"title\")", "P@1: [a](u \"title\")\n");
    MD("[a]", "P@1: [a]\n");
    MD("[a] (u)", "P@1: [a] (u)\n");
    MD("[a](", "P@1: [a](\n");
    MD("[a](u", "P@1: [a](u\n");
    MD("[a](u(v)", "P@1: [a](u(v)\n");
    MD("![img](u)", "P@1: !<a u>img</a>\n");
    /* Brackets. */
    MD("[a [b] c](u)", "P@1: <a u>a [b] c</a>\n");
    MD("[[a](u)", "P@1: [<a u>a</a>\n");
    MD("[a](u))", "P@1: <a u>a</a>)\n");
    MD("]a[", "P@1: ]a[\n");
    MD("[[[", "P@1: [[[\n");
    MD("]]]", "P@1: ]]]\n");
    MD("[a [b](u1) c](u2)", "P@1: [a <a u1>b</a> c](u2)\n");
    MD("[](u)", "P@1: \n");
    /* Emphasis can't cross the link boundary. */
    MD("*[a*](u)", "P@1: *<a u>a*</a>\n");
    MD("[*a](u)*", "P@1: <a u>*a</a>*\n");
    MD("*a [b* c]", "P@1: <i>a [b</i> c]\n");
    /* Blanks at the ends of a block's content are trimmed, inside links too. */
    MD("[\nb](u)", "P@1: <a u>b</a>\n");
    MD("[a ](u)", "P@1: <a u>a</a>\n");
    MD("a [ ](u)", "P@1: a\n");
    MD("[\\\n](u)b", "P@1: b\n");
    MD("x [a\\\n](u)", "P@1: x <a u>a</a>\n");
    /* Adjacent links to the same url merge into one span. */
    MD("[a](u)[b](u)", "P@1: <a u>ab</a>\n");
    MD("[a](u)[b](v)", "P@1: <a u>a</a><a v>b</a>\n");
    MD("[a](u)b", "P@1: <a u>a</a>b\n");
}

static void test_crlf(void)
{
    MD("a\r\nb\r\n\r\n# h\r\n", "P@1: a b\nH1@4: h\n");
    MD("a\rb\r\rc", "P@1: a b\nP@4: c\n");
    MD("```\r\nx\r\n\r\ny\r\n```\r\nz", "C@1[]: x\\n\\ny\\n\nP@6: z\n");
    MD("```\rx\r```", "C@1[]: x\\n\n");
    MD("a  \r\nb", "P@1: a<br>b\n");
    MD("a\\\r\nb", "P@1: a<br>b\n");
    MD("- a\r\n  - b\r\n", "L0-@1: a\nL1-@2: b\n");
    MD("\r\n\r\n@examples\r\n", "X@3\n");
    MD("a\n\r\nb", "P@1: a\nP@3: b\n");
    MD("a\r\r\nb", "P@1: a\nP@3: b\n");
}

static void test_nul_bytes(void)
{
    MDN("a\0b", 3, "P@1: a\xEF\xBF\xBD" "b\n");
    MDN("\0", 1, "P@1: \xEF\xBF\xBD\n");
    MDN("```\n\0\n```", 9, "C@1[]: \xEF\xBF\xBD\\n\n");
    MDN("`\0`", 3, "P@1: <c>\xEF\xBF\xBD</c>\n");
    MDN("# a\0\n\0", 6, "H1@1: a\xEF\xBF\xBD\nP@2: \xEF\xBF\xBD\n");
    MDN("[a](\0)", 6, "P@1: <a \xEF\xBF\xBD>a</a>\n");
    MDN("*\0*", 3, "P@1: <i>\xEF\xBF\xBD</i>\n");
}

static void test_line_numbers(void)
{
    MD("\n\n# h\n\npara\nline2\n\n- a\n- b\n\n```\nx\n```\n---\n@examples\n"
       "> q\n> r\n>\n> s\n\n1. n",
       "H1@3: h\nP@5: para line2\nL0-@8: a\nL0-@9: b\nC@11[]: x\\n\n"
       "R@14\nX@15\nQ@16: q r\nQ@19: s\nL0.1@21: n\n");
}

static void test_example_statement(void)
{
    MD("Given an array of integers `nums` and an integer `target`, return the\n"
       "**indices** of the two numbers that add up to `target`.\n"
       "\n"
       "Each input has *exactly one* solution, and you may not use the same "
       "element\n"
       "twice. You can return the answer in any order.\n"
       "\n"
       "@examples\n"
       "\n"
       "## Constraints\n"
       "\n"
       "- `2 <= nums.length <= 10^4`\n"
       "- `-10^9 <= nums[i] <= 10^9`\n"
       "- Exactly one valid answer exists.\n"
       "\n"
       "## Follow-up\n"
       "\n"
       "Can you find an algorithm that runs in less than `O(n^2)` time?\n",
       "P@1: Given an array of integers <c>nums</c> and an integer "
       "<c>target</c>, return the <b>indices</b> of the two numbers that add "
       "up to <c>target</c>.\n"
       "P@4: Each input has <i>exactly one</i> solution, and you may not use "
       "the same element twice. You can return the answer in any order.\n"
       "X@7\n"
       "H2@9: Constraints\n"
       "L0-@11: <c>2 <= nums.length <= 10^4</c>\n"
       "L0-@12: <c>-10^9 <= nums[i] <= 10^9</c>\n"
       "L0-@13: Exactly one valid answer exists.\n"
       "H2@15: Follow-up\n"
       "P@17: Can you find an algorithm that runs in less than <c>O(n^2)</c> "
       "time?\n");
}

static void test_span_fields(void)
{
    lc_md_doc *doc = NULL;
    const lc_md_block *b;

    CHECK_INT(lc_md_parse("a **b** [c](u)  \nd", 18, &doc), LC_OK);
    CHECK_INT(doc->nblocks, 1);
    b = &doc->blocks[0];
    CHECK_INT(b->kind, LC_MD_PARAGRAPH);
    CHECK(b->lang == NULL && b->text == NULL);
    CHECK_INT(b->nspans, 6);
    if (b->nspans == 6) {
        CHECK_INT(b->spans[0].flags, 0);
        CHECK_STR(b->spans[0].text, "a ");
        CHECK_INT(b->spans[0].len, 2);
        CHECK(b->spans[0].url == NULL);
        CHECK_INT(b->spans[1].flags, LC_MD_SPAN_STRONG);
        CHECK_STR(b->spans[1].text, "b");
        CHECK_INT(b->spans[2].flags, 0);
        CHECK_STR(b->spans[2].text, " ");
        CHECK_INT(b->spans[3].flags, LC_MD_SPAN_LINK);
        CHECK_STR(b->spans[3].text, "c");
        CHECK_STR(b->spans[3].url, "u");
        CHECK_INT(b->spans[4].flags, LC_MD_SPAN_BREAK);
        CHECK_STR(b->spans[4].text, "");
        CHECK_INT(b->spans[4].len, 0);
        CHECK(b->spans[4].url == NULL);
        CHECK_STR(b->spans[5].text, "d");
    }
    lc_md_free(doc);

    CHECK_INT(lc_md_parse("~~~ c++ x\nint a;\n~~~", 20, &doc), LC_OK);
    CHECK_INT(doc->nblocks, 1);
    b = &doc->blocks[0];
    CHECK_INT(b->kind, LC_MD_CODE);
    CHECK_STR(b->lang, "c++");
    CHECK_STR(b->text, "int a;\n");
    CHECK_INT(b->len, 7);
    CHECK_INT(b->nspans, 0);
    lc_md_free(doc);

    CHECK_INT(lc_md_parse("```\n```", 7, &doc), LC_OK);
    CHECK_STR(doc->blocks[0].lang, "");
    CHECK_STR(doc->blocks[0].text, "");
    CHECK_INT(doc->blocks[0].len, 0);
    lc_md_free(doc);

    /* The input need not be NUL-terminated. */
    CHECK_INT(lc_md_parse("# abcdef", 5, &doc), LC_OK);
    CHECK_INT(doc->nblocks, 1);
    CHECK_STR(doc->blocks[0].spans[0].text, "abc");
    lc_md_free(doc);
}

/*
 * Input that ends inside a construct. render() parses from an exact-size
 * copy, so under ASan or valgrind a read past the end is caught.
 */
static void test_buffer_end(void)
{
    MD("#", "P@1: #\n");
    MD("x\n######", "P@1: x ######\n");
    MD("# a #", "H1@1: a\n");
    MD("-", "P@1: -\n");
    MD("x\n*", "P@1: x *\n");
    MD("- a\n  +", "L0-@1: a +\n");
    MD("1.", "P@1: 1.\n");
    MD("x\n1)", "P@1: x 1)\n");
    MD("123456789.", "P@1: 123456789.\n");
    MD("1234567890", "P@1: 1234567890\n");
    MD(">", "");
    MD("x\n> ", "P@1: x\n");
    MD("```", "C@1[]: \n");
    MD("~~~~", "C@1[]: \n");
    MD("``` c", "C@1[c]: \n");
    MD("```\n ", "C@1[]:  \\n\n");
    MD("```\n   ", "C@1[]:    \\n\n");
    MD("```\n``", "C@1[]: ``\\n\n");
    MD("~~~\n   ~~~", "C@1[]: \n");
    MD("@examples", "X@1\n");
    MD("@example", "P@1: @example\n");
    MD("***", "R@1\n");
    MD("**", "P@1: **\n");
    MD("x\\", "P@1: x\\\n");
    MD("x `y", "P@1: x `y\n");
    MD("[a](", "P@1: [a](\n");
    MD("[a](u\\", "P@1: [a](u\\\n");
    MD("[a]", "P@1: [a]\n");
}

/*
 * Larger inputs, so that buffers grow several times and the arena needs
 * more than one block. render() runs each with every allocation failing.
 */
static void check_big(const lc_str *src)
{
    char *got;

    CHECK(!src->failed);
    got = render(lc_str_cstr(src), src->len);
    CHECK(strcmp(got, "<error>") != 0);
    free(got);
}

static void test_alloc_failures(void)
{
    lc_str src = LC_STR_INIT;
    int i;

    for (i = 0; i < 60; i++)
        lc_str_printf(&src, "# Part %d\n\nSome *text*, `code` and a [link](u%d)"
                      "  \nmore **text**\n- item\n  1. sub\n> quote\n\n"
                      "```c\nint x%d;\n```\n---\n@examples\n", i, i, i);
    check_big(&src);
    lc_str_clear(&src);
    for (i = 0; i < 1000; i++)
        lc_str_printf(&src, "w%d *em* `c` [l](u%d) __s__\n", i, i % 7);
    check_big(&src);
    lc_str_free(&src);
}

/* ------------------------------------------------------------------ */
/* Fuzzing and hostile input                                           */
/* ------------------------------------------------------------------ */

static unsigned long long lcg_state = 12345;

static unsigned lcg(void)
{
    lcg_state = lcg_state * 6364136223846793005ULL + 1442695040888963407ULL;
    return (unsigned)(lcg_state >> 33);
}

static void test_fuzz(void)
{
    static const char *const tokens[] = {
        "```", "~~~", "@examples", "1. ", "2) ", "- ", "* ", "+ ", "> ",
        "    ", "  ", "# ", "### ", "**", "__", "[a](b)", "](", "\\",
        "\r\n", "\n\n", "`` ", "---", "\t", "*a*", "_b_", "[", "](u)",
        "  \n", "\\\n", "123456789. ", "#######", " @examples ", "x_y_z"
    };
    static const char punct[] = "*_`[]()\\#>-+.~!@ \t\n\n\n\r0123456789";
    static const char letters[] = "abcxyz";
    char buf[512];
    int iter, failed = 0;

    for (iter = 0; iter < 6000 && !failed; iter++) {
        size_t n = 0, want = lcg() % 400;
        lc_md_doc *doc = NULL;
        char *copy;
        int rc;

        while (n < want) {
            unsigned r = lcg() % 100;

            if (r < 20) {
                size_t ntok = sizeof tokens / sizeof *tokens;
                const char *t = tokens[lcg() % ntok];
                size_t tl = strlen(t);
                if (n + tl > sizeof buf)
                    break;
                memcpy(buf + n, t, tl);
                n += tl;
            } else if (r < 65) {
                buf[n++] = punct[lcg() % (sizeof punct - 1)];
            } else if (r < 95) {
                buf[n++] = letters[lcg() % (sizeof letters - 1)];
            } else {
                buf[n++] = (char)(lcg() & 0xff);
            }
        }
        copy = copy_exact(buf, n);
        CHECK(copy != NULL);
        if (!copy)
            break;
        rc = lc_md_parse(copy, n, &doc);
        CHECK_INT(rc, LC_OK);
        if (rc != LC_OK) {
            free(copy);
            failed = 1;
            break;
        }
        if (!doc_ok(doc, copy, n)) {
            CHECK(!"fuzz invariant");
            failed = 1;
        } else if (iter % 25 == 0) {
            char *got = render_doc(doc);

            if (!got || !oom_ok(copy, n, got)) {
                CHECK(!"fuzz allocation failures");
                failed = 1;
            }
            free(got);
        }
        lc_md_free(doc);
        free(copy);
    }
    CHECK(!failed);
}

/*
 * Parses `prefix` followed by `unit` repeated to at least `total` bytes,
 * from a buffer of exactly that size.
 */
static void parse_big(const char *prefix, const char *unit, size_t total)
{
    size_t pl = strlen(prefix), ul = strlen(unit), k;
    size_t reps = (total + ul - 1) / ul, n = pl + reps * ul;
    char *buf = malloc(n);
    lc_md_doc *doc = NULL;
    int rc;

    CHECK(buf != NULL);
    if (!buf)
        return;
    memcpy(buf, prefix, pl);
    for (k = 0; k < reps; k++)
        memcpy(buf + pl + k * ul, unit, ul);
    rc = lc_md_parse(buf, n, &doc);
    CHECK_INT(rc, LC_OK);
    if (rc == LC_OK) {
        CHECK(doc_ok(doc, buf, n));
        lc_md_free(doc);
    }
    free(buf);
}

static void test_pathological(void)
{
    /* The inputs named in the requirements, at 1 MB each. */
    static const char *const big_units[] = {"*", "_", "[", "`", "> "};
    /* Everything else that has hurt markdown parsers, at 128 KB each. */
    static const char *const units[] = {
        "]", "\\", "(", "#", "-", "\r", "*a", "_a ", "a_", "**a",
        "*a **a ", "`a", "``a`", "` ``", "[a](", "[](", "[a]", "](", "[*",
        "*[", "a*", "*_", "_*", "[a](b)", "![", "  \n", "a\n", "\\\n",
        "> \n", "- ", "1. ", "- x\n", "```\n", "*a\n", "[\n", "`\n",
        "a  \n"
    };
    const size_t mb = (size_t)1 << 20;
    size_t i;

    for (i = 0; i < sizeof big_units / sizeof *big_units; i++) {
        parse_big("", big_units[i], mb);  /* as blocks */
        parse_big("a", big_units[i], mb); /* as inline text in one paragraph */
    }
    for (i = 0; i < sizeof units / sizeof *units; i++) {
        parse_big("", units[i], mb / 8);
        parse_big("a", units[i], mb / 8);
    }

    /* NUL bytes, each becoming three bytes. */
    {
        lc_md_doc *doc = NULL;
        char *buf = calloc(1, mb);

        CHECK(buf != NULL);
        if (buf) {
            CHECK_INT(lc_md_parse(buf, mb, &doc), LC_OK);
            if (doc) {
                CHECK(doc_ok(doc, buf, mb));
                CHECK_INT(doc->nblocks, 1);
                CHECK_INT(doc->blocks[0].spans[0].len, 3 * mb);
            }
            lc_md_free(doc);
            free(buf);
        }
    }

    /* Nested emphasis, about 170k levels deep. */
    {
        size_t n = 0, k, half = mb / 6;
        char *buf = malloc(6 * half);
        lc_md_doc *doc = NULL;

        CHECK(buf != NULL);
        if (buf) {
            for (k = 0; k < half; k++, n += 3)
                memcpy(buf + n, "*a ", 3);
            for (k = 0; k < half; k++, n += 3)
                memcpy(buf + n, " a*", 3);
            CHECK_INT(lc_md_parse(buf, n, &doc), LC_OK);
            if (doc) {
                CHECK(doc_ok(doc, buf, n));
                CHECK_INT(doc->nblocks, 1);
            }
            lc_md_free(doc);
            free(buf);
        }
    }

    /* List markers indented ever deeper. */
    {
        lc_str s = LC_STR_INIT;
        lc_md_doc *doc = NULL;
        char *buf;
        int k;

        for (k = 0; s.len < mb && !s.failed; k++)
            lc_str_printf(&s, "%*s- x\n", 2 * k, "");
        CHECK(!s.failed);
        buf = copy_exact(lc_str_cstr(&s), s.len);
        CHECK(buf != NULL);
        if (buf) {
            CHECK_INT(lc_md_parse(buf, s.len, &doc), LC_OK);
            if (doc) {
                CHECK(doc_ok(doc, buf, s.len));
                CHECK_INT(doc->blocks[doc->nblocks - 1].depth, 15);
            }
            lc_md_free(doc);
            free(buf);
        }
        lc_str_free(&s);
    }
}

int main(void)
{
    RUN(test_empty);
    RUN(test_paragraphs);
    RUN(test_hard_breaks);
    RUN(test_headings);
    RUN(test_code_blocks);
    RUN(test_lists);
    RUN(test_list_nesting);
    RUN(test_list_numbering);
    RUN(test_interruptions);
    RUN(test_quotes);
    RUN(test_rules);
    RUN(test_examples_placeholder);
    RUN(test_emphasis);
    RUN(test_code_spans);
    RUN(test_escapes);
    RUN(test_links);
    RUN(test_crlf);
    RUN(test_nul_bytes);
    RUN(test_line_numbers);
    RUN(test_example_statement);
    RUN(test_span_fields);
    RUN(test_buffer_end);
    RUN(test_alloc_failures);
    RUN(test_fuzz);
    RUN(test_pathological);
    return test_report();
}
