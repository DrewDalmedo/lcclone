/*
 * lc.h: public API of liblc, the lcclone core.
 *
 * Rules every front-end can rely on (docs/PLAN.md section 4.1):
 *   - the core never prints, never calls exit() and never reads stdin;
 *   - there is no global state: everything lives in an lc_ctx;
 *   - functions that can fail return an int status (LC_OK on success) and
 *     leave a message in the context, readable with lc_last_error().
 *
 * An lc_ctx may be used by one thread at a time. To work on two threads,
 * open one context per thread.
 */
#ifndef LC_H
#define LC_H

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LC_VERSION "0.1.0-dev"

/* Version of the `lc --json` output format. */
#define LC_JSON_SCHEMA 1

#if defined(__GNUC__) || defined(__clang__)
#define LC_PRINTF(fmt, args) __attribute__((format(printf, fmt, args)))
#else
#define LC_PRINTF(fmt, args)
#endif

const char *lc_version(void);

/* ------------------------------------------------------------------ */
/* Status codes                                                        */
/* ------------------------------------------------------------------ */

enum {
    LC_OK = 0,
    LC_ERR_NOMEM,    /* out of memory */
    LC_ERR_IO,       /* a file or directory could not be read or written */
    LC_ERR_PARSE,    /* a file is malformed */
    LC_ERR_NOTFOUND, /* no such problem, file or directory */
    LC_ERR_INVALID   /* the caller passed a bad argument or misused an API */
};

/* ------------------------------------------------------------------ */
/* Growable strings                                                    */
/* ------------------------------------------------------------------ */

/*
 * A growable byte buffer. Zero-initialize it (or use LC_STR_INIT /
 * lc_str_init) before use and release it with lc_str_free.
 *
 * `data` is NULL until something is appended. After any successful append
 * it is NUL-terminated, and `len` excludes the terminator.
 *
 * Errors are sticky: once an allocation fails, `failed` is set and every
 * later append does nothing and returns LC_ERR_NOMEM. That allows building
 * a long string and checking for failure once at the end.
 */
typedef struct lc_str {
    char *data;
    size_t len;
    size_t cap;
    int failed;
} lc_str;

#define LC_STR_INIT {NULL, 0, 0, 0}

void lc_str_init(lc_str *s);
void lc_str_free(lc_str *s);                 /* frees memory, back to the initial state */
void lc_str_clear(lc_str *s);                /* len = 0 and failed = 0; keeps the memory */
int lc_str_reserve(lc_str *s, size_t extra); /* room for `extra` more bytes plus a NUL */
int lc_str_append(lc_str *s, const char *p, size_t n);
int lc_str_puts(lc_str *s, const char *p);
int lc_str_putc(lc_str *s, int c);
int lc_str_printf(lc_str *s, const char *fmt, ...) LC_PRINTF(2, 3);
int lc_str_vprintf(lc_str *s, const char *fmt, va_list ap);
const char *lc_str_cstr(const lc_str *s);    /* data, or "" when data is NULL */

/* ------------------------------------------------------------------ */
/* Context                                                             */
/* ------------------------------------------------------------------ */

typedef struct lc_ctx lc_ctx;

typedef struct lc_options {
    /*
     * Directory of problem packs to use. When NULL the core searches, in
     * order: $LC_PROBLEMS, $XDG_DATA_HOME/lcclone/problems (default
     * ~/.local/share/lcclone/problems), then $(PREFIX)/share/lcclone/problems.
     */
    const char *problems_dir;
} lc_options;

void lc_options_init(lc_options *opt);

/* Creates a context. `opt` may be NULL. Returns NULL only when out of memory. */
lc_ctx *lc_open(const lc_options *opt);
void lc_close(lc_ctx *ctx);

/*
 * Finds the problem directory and indexes every pack in it. Call it once,
 * before the lc_problem_* functions.
 *
 * A pack that fails to load is skipped and recorded as a warning; it does
 * not make lc_load fail. lc_load fails only when no problem directory can be
 * found or read, or when memory runs out.
 */
int lc_load(lc_ctx *ctx);

/* The problem directory chosen by lc_load (an absolute path), or NULL. */
const char *lc_problems_dir(const lc_ctx *ctx);

/* Message for the most recent failure, or "" if there was none. */
const char *lc_last_error(const lc_ctx *ctx);

/* Non-fatal problems, such as packs skipped by lc_load. */
size_t lc_warning_count(const lc_ctx *ctx);
const char *lc_warning_at(const lc_ctx *ctx, size_t i);

/* ------------------------------------------------------------------ */
/* Problems                                                            */
/* ------------------------------------------------------------------ */

typedef enum {
    LC_DIFF_UNKNOWN = 0,
    LC_EASY,
    LC_MEDIUM,
    LC_HARD
} lc_difficulty;

/* "easy", "medium", "hard", or "unknown". */
const char *lc_difficulty_name(lc_difficulty d);
/* Parses "easy", "medium" or "hard", ignoring case; LC_DIFF_UNKNOWN otherwise. */
lc_difficulty lc_difficulty_from_name(const char *s);

/* One `key: value` line from a problem.md header. */
typedef struct lc_header_field {
    const char *key;
    const char *value; /* trimmed; may be "" */
    int line;          /* 1-based line number in problem.md */
} lc_header_field;

/*
 * A loaded problem pack (docs/problem-format.md). Everything it points to is
 * owned by the context and stays valid until lc_close.
 */
typedef struct lc_problem {
    int id;                   /* the LeetCode number */
    const char *slug;
    const char *title;
    lc_difficulty difficulty;
    const char *const *tags;
    size_t ntags;
    const char *signature;    /* raw `signature:` value, or NULL */
    /*
     * Names for the input lines of a test case, used to show examples as
     * "nums = [1,2], target = 3": the signature's parameter names followed
     * by the names from `hidden:`. Empty when the pack has no signature.
     */
    const char *const *arg_names;
    size_t narg_names;
    int time_limit_ms;        /* default 1000 */
    int memory_limit_mb;      /* default 256 */
    const char *dir;          /* absolute path of the pack directory */
    const char *statement;    /* markdown text after the '---' line */
    size_t statement_len;
    int statement_line;       /* line of problem.md where the statement starts */
    const lc_header_field *fields; /* every header line, in file order */
    size_t nfields;
} lc_problem;

/* Problems sorted by id. */
size_t lc_problem_count(const lc_ctx *ctx);
const lc_problem *lc_problem_at(const lc_ctx *ctx, size_t i);

/*
 * Finds a problem by slug ("two-sum"), number ("1" or "0001") or pack
 * directory name ("0001-two-sum"). Returns NULL and sets the last error
 * when nothing matches.
 */
const lc_problem *lc_problem_find(lc_ctx *ctx, const char *key);

/* Value of a header field, or NULL if the header has no such key. */
const char *lc_problem_field(const lc_problem *p, const char *key);

/* 1 if the problem has this tag (exact match), else 0. */
int lc_problem_has_tag(const lc_problem *p, const char *tag);

/* ------------------------------------------------------------------ */
/* Test cases (tests.txt)                                              */
/* ------------------------------------------------------------------ */

/* Values are kept as raw literal text; parsing them is milestone M1. */
typedef struct lc_case {
    const char *const *args;  /* one literal per input line */
    size_t nargs;
    const char *expected;     /* literal after "=>" */
    const char *explanation;  /* text after "@example", or NULL */
    int example;              /* 1 if marked with @example */
    int line;                 /* line of the case's first input */
} lc_case;

typedef struct lc_cases {
    const lc_case *items;
    size_t count;
    void *priv_;              /* owned memory; do not touch */
} lc_cases;

/*
 * Reads the pack's tests.txt. A missing tests.txt gives an empty list.
 * On success *out must be released with lc_cases_free.
 */
int lc_load_cases(lc_ctx *ctx, const lc_problem *p, lc_cases **out);

/* Parses tests.txt text. `filename` is used in error messages. */
int lc_parse_cases(lc_ctx *ctx, const char *text, size_t len,
                   const char *filename, lc_cases **out);

void lc_cases_free(lc_cases *cases);

/* ------------------------------------------------------------------ */
/* Markdown (the subset described in docs/problem-format.md)           */
/* ------------------------------------------------------------------ */

typedef enum {
    LC_MD_PARAGRAPH,
    LC_MD_HEADING,   /* level 1-6 */
    LC_MD_CODE,      /* fenced code block: lang, text */
    LC_MD_LIST_ITEM, /* ordered, number, depth */
    LC_MD_QUOTE,     /* a paragraph inside a '>' block quote */
    LC_MD_RULE,      /* thematic break */
    LC_MD_EXAMPLES   /* the "@examples" placeholder line */
} lc_md_kind;

/* Span flags. LC_MD_SPAN_CODE is never combined with STRONG or EM. */
enum {
    LC_MD_SPAN_STRONG = 1 << 0,
    LC_MD_SPAN_EM = 1 << 1,
    LC_MD_SPAN_CODE = 1 << 2,
    LC_MD_SPAN_LINK = 1 << 3, /* url is set */
    LC_MD_SPAN_BREAK = 1 << 4 /* hard line break; text is "" */
};

typedef struct lc_md_span {
    unsigned flags;
    const char *text; /* NUL-terminated, escapes already removed */
    size_t len;
    const char *url;  /* only for LC_MD_SPAN_LINK, else NULL */
} lc_md_span;

typedef struct lc_md_block {
    lc_md_kind kind;
    int level;                 /* heading level */
    int ordered;               /* list item: 1 if numbered */
    int number;                /* list item: its number when ordered */
    int depth;                 /* list item: nesting depth, 0 = top level */
    const char *lang;          /* code block language: the first word of the
                                  fence's info string, "" if none */
    const char *text;          /* code block contents, ending in '\n' unless empty */
    size_t len;
    const lc_md_span *spans;   /* inline content: paragraph, heading, list item, quote */
    size_t nspans;
    int line;                  /* 1-based line in the markdown text; only
                                  "\n" starts a new number (a lone "\r" ends
                                  a markdown line but keeps the number) */
} lc_md_block;

typedef struct lc_md_doc {
    const lc_md_block *blocks;
    size_t nblocks;
    void *priv_;               /* owned memory; do not touch */
} lc_md_doc;

/*
 * Parses markdown. Any input is accepted (unknown syntax becomes plain
 * text), so this fails only when memory runs out. On success *out must be
 * released with lc_md_free.
 */
int lc_md_parse(const char *text, size_t len, lc_md_doc **out);
void lc_md_free(lc_md_doc *doc);

/* ------------------------------------------------------------------ */
/* JSON writer                                                         */
/* ------------------------------------------------------------------ */

#define LC_JSON_MAX_DEPTH 64

/*
 * Writes compact JSON into an lc_str. Calls never fail on their own: the
 * first error (misuse, too much nesting, out of memory) is remembered and
 * later calls do nothing. lc_json_finish reports it.
 *
 * Strings are written as UTF-8. Invalid UTF-8 bytes become U+FFFD and
 * control characters are escaped. NaN and infinities are written as null.
 *
 * The fields are private.
 */
typedef struct lc_json {
    lc_str *out;
    int err;
    int depth;
    unsigned char kind[LC_JSON_MAX_DEPTH];  /* 'o' object or 'a' array */
    unsigned char count[LC_JSON_MAX_DEPTH]; /* 0 until the first member */
    unsigned char after_key;                /* a key was written; a value must follow */
    unsigned char done;                     /* a complete top-level value was written */
} lc_json;

void lc_json_init(lc_json *j, lc_str *out);
void lc_json_begin_object(lc_json *j);
void lc_json_end_object(lc_json *j);
void lc_json_begin_array(lc_json *j);
void lc_json_end_array(lc_json *j);
void lc_json_key(lc_json *j, const char *key);
void lc_json_string(lc_json *j, const char *s); /* NULL writes null */
void lc_json_string_n(lc_json *j, const char *s, size_t n);
void lc_json_int(lc_json *j, long long v);
void lc_json_double(lc_json *j, double v);
void lc_json_bool(lc_json *j, int v);
void lc_json_null(lc_json *j);
/* LC_OK when exactly one complete value was written and nothing failed. */
int lc_json_finish(lc_json *j);

#ifdef __cplusplus
}
#endif

#endif /* LC_H */
