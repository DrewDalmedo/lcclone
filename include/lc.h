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

#include <signal.h>
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
/* Types (docs/problem-format.md, "Types")                             */
/* ------------------------------------------------------------------ */

/* What a type is made of, before any array levels. */
typedef enum {
    LC_T_VOID = 0, /* only as a return type */
    LC_T_INT,      /* 32-bit signed */
    LC_T_LONG,     /* 64-bit signed */
    LC_T_UINT32,   /* 32-bit unsigned */
    LC_T_DOUBLE,
    LC_T_BOOL,
    LC_T_CHAR,     /* one ASCII character, written "a" */
    LC_T_STRING,
    LC_T_LIST,     /* ListNode: a singly linked list of ints */
    LC_T_TREE,     /* TreeNode: a binary tree of ints */
    LC_T_NARY      /* NaryNode: a tree of ints with any number of children */
} lc_kind;

#define LC_MAX_DIMS 2       /* int[][] is as deep as arrays go */
#define LC_TYPE_NAME_MAX 16 /* room for any type's name and its NUL */

typedef struct lc_type {
    lc_kind kind;
    int dims; /* array levels: 0 for int, 1 for int[], 2 for int[][] */
} lc_type;

/*
 * Parses a type name such as "int", "string[][]" or "TreeNode" (no spaces).
 * Returns LC_ERR_PARSE for an unknown name or a combination that isn't
 * supported (ListNode[][], void[]). "void" is accepted; whether it is
 * allowed depends on where the type is used.
 */
int lc_type_parse(const char *s, size_t n, lc_type *out);

/* Writes the type's name into buf (LC_TYPE_NAME_MAX bytes); returns buf. */
const char *lc_type_name(lc_type t, char *buf);

/* One parameter of a signature, or one `hidden:` input. */
typedef struct lc_param {
    lc_type type;
    const char *name;
} lc_param;

/* A parsed `signature:` such as int[] twoSum(int[] nums, int target). */
typedef struct lc_sig {
    lc_type ret;
    const char *name;
    const lc_param *params;
    size_t nparams;
} lc_sig;

/* How a result is checked against the expected one (`compare:`). */
typedef enum {
    LC_CMP_EXACT = 0,
    LC_CMP_FLOAT,     /* doubles may differ by up to `tolerance` */
    LC_CMP_UNORDERED, /* order is ignored at the levels in `levels` */
    LC_CMP_CHECKER    /* the pack's checker.c decides (milestone M8) */
} lc_cmp_mode;

typedef struct lc_compare {
    lc_cmp_mode mode;
    double tolerance; /* LC_CMP_FLOAT */
    unsigned levels;  /* LC_CMP_UNORDERED: bit k set ignores the order of
                         the arrays at nesting level k (0 = outermost) */
} lc_compare;

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
    const lc_sig *sig;        /* the parsed signature, or NULL */
    const lc_param *hidden;   /* the parsed `hidden:` inputs */
    size_t nhidden;
    /*
     * How results are compared: the `compare:` field, or by default exact,
     * except that a result made of doubles defaults to a tolerance of 1e-5.
     */
    lc_compare compare;
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

/*
 * Values are kept as raw literal text; lc_case_values parses them against
 * the problem's signature.
 */
typedef struct lc_case {
    const char *const *args;  /* one literal per input line; none for "()" */
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
/* Values (docs/problem-format.md, "Literals")                         */
/* ------------------------------------------------------------------ */

typedef enum {
    LC_V_NULL = 0,
    LC_V_BOOL,
    LC_V_INT,
    LC_V_FLOAT,
    LC_V_STRING,
    LC_V_ARRAY
} lc_vkind;

/*
 * A parsed value. What each type becomes:
 *   int, long, uint32      LC_V_INT
 *   double                 LC_V_FLOAT (an integer literal is converted)
 *   bool                   LC_V_BOOL
 *   char, string           LC_V_STRING (a char is one byte long)
 *   T[]                    LC_V_ARRAY of T
 *   ListNode               LC_V_ARRAY of LC_V_INT
 *   TreeNode, NaryNode     LC_V_ARRAY of LC_V_INT and LC_V_NULL, in
 *                          LeetCode's level order without trailing nulls
 */
typedef struct lc_value lc_value;
struct lc_value {
    lc_vkind kind;
    size_t len;                /* LC_V_STRING: bytes; LC_V_ARRAY: items */
    union {
        int b;                 /* LC_V_BOOL: 0 or 1 */
        long long i;           /* LC_V_INT */
        double f;              /* LC_V_FLOAT */
        const char *s;         /* LC_V_STRING: len bytes, then a NUL */
        const lc_value *items; /* LC_V_ARRAY */
    } u;
};

/* Owns parsed values; lc_pool_free releases them all at once. */
typedef struct lc_pool lc_pool;

lc_pool *lc_pool_new(void); /* NULL when out of memory */
void lc_pool_free(lc_pool *pool);

/*
 * Parses a literal and checks it against `type` (NULL accepts any
 * literal). The value lives in `pool`. A malformed literal or one that
 * doesn't fit the type gives LC_ERR_PARSE and a message such as
 * "column 5: expected ',' or ']'", where columns count bytes from 1.
 */
int lc_value_parse(lc_ctx *ctx, lc_pool *pool, const lc_type *type,
                   const char *text, size_t len, const lc_value **out);

/*
 * Appends the value as a literal, with no spaces: [1,2], "a\"b", 2.5,
 * [1,null,2]. Doubles print as the shortest text that reads back the same,
 * with ".0" added to whole numbers. A string that isn't valid UTF-8 (only
 * a program's result can be) shows its stray bytes as \xHH, which is not
 * literal syntax; NaN and infinities print as NaN, Infinity and -Infinity.
 */
int lc_value_print(const lc_value *v, lc_str *out);

/*
 * Sets *match to 1 if `got` matches `expected` under `cmp`, else 0. Both
 * values must fit the same type. LC_CMP_CHECKER is not handled here and
 * gives LC_ERR_INVALID.
 */
int lc_value_match(lc_ctx *ctx, const lc_compare *cmp,
                   const lc_value *expected, const lc_value *got, int *match);

/*
 * Parses a case's literals: the inputs against the signature's parameters
 * and then the `hidden:` inputs, and the expected output against the
 * return type. Without a signature, or when it returns void, values are
 * parsed without a type. *args points to an array of c->nargs values, and
 * everything lives in `pool`. Errors name the file, the case's line and
 * the input, e.g.
 * ".../tests.txt:12: target: column 1: expected int, got a string".
 */
int lc_case_values(lc_ctx *ctx, const lc_problem *p, const lc_case *c,
                   lc_pool *pool, const lc_value **args,
                   const lc_value **expected);

/* ------------------------------------------------------------------ */
/* Wire format between lc and the harness (docs/wire.md)               */
/* ------------------------------------------------------------------ */

/*
 * Appends `v` in the wire format for `type`, with tokens separated by
 * single spaces and no trailing separator. `v` must fit the type.
 */
int lc_wire_encode(lc_ctx *ctx, const lc_type *type, const lc_value *v,
                   lc_str *out);

/*
 * Reads one value of `type` from text[*pos..len) and advances *pos past
 * it. Leading whitespace is skipped. Malformed text gives LC_ERR_PARSE
 * with a message naming the byte offset. Whatever the text says, this
 * allocates at most a fixed multiple of the text's size.
 */
int lc_wire_decode(lc_ctx *ctx, lc_pool *pool, const lc_type *type,
                   const char *text, size_t len, size_t *pos,
                   const lc_value **out);

/* ------------------------------------------------------------------ */
/* Judging (docs/judging.md)                                           */
/* ------------------------------------------------------------------ */

/*
 * Judging runs a solution once per test case, as a child process in its
 * own process group. That asks two things of the front-end's process:
 *
 *   - SIGPIPE must be ignored (signal(SIGPIPE, SIG_IGN)). A program that
 *     stops reading its input then makes the core's writes fail with
 *     EPIPE, which it handles, instead of killing the front-end.
 *   - SIGCHLD must not be set to SIG_IGN, or the system reaps the programs
 *     before the core can collect their exit status and resource usage.
 *
 * Because the programs are not in the terminal's foreground process group,
 * ^C reaches only the front-end. A CLI should catch SIGINT and set the
 * cancel flag in lc_hooks.
 */

typedef enum {
    LC_VERDICT_NONE = 0,      /* no verdict: the run was cancelled first */
    LC_ACCEPTED,
    LC_WRONG_ANSWER,
    LC_COMPILE_ERROR,
    LC_RUNTIME_ERROR,
    LC_TIME_LIMIT_EXCEEDED,
    LC_MEMORY_LIMIT_EXCEEDED,
    LC_OUTPUT_LIMIT_EXCEEDED
} lc_verdict;

/* "Accepted", "Wrong Answer", ...; "No Verdict" for LC_VERDICT_NONE. */
const char *lc_verdict_name(lc_verdict v);
/* "accepted", "wrong_answer", ...; "none" for LC_VERDICT_NONE. These ids
 * are stable, for JSON output. */
const char *lc_verdict_id(lc_verdict v);
/* "SIGSEGV" and the like, or NULL for a number it doesn't know. */
const char *lc_signal_name(int sig);

/* How much of a program's stdout and stderr a case result keeps. */
#define LC_REPORT_TEXT_MAX 65536

/*
 * How one test case went. Strings are NUL-terminated and owned by the
 * report they belong to.
 */
typedef struct lc_case_result {
    size_t index;             /* position in the list of cases that was run */
    int line;                 /* line of the case in tests.txt */
    int example;              /* 1 for an @example case */
    lc_verdict verdict;
    /*
     * Why the verdict, for anything but Accepted and Wrong Answer, e.g.
     * "killed by SIGSEGV" or "used 1630 ms of CPU time; the limit is
     * 1000 ms". NULL for Accepted and Wrong Answer.
     */
    const char *message;
    const char *const *inputs; /* the case's input literals, as written */
    size_t ninputs;
    const char *expected;     /* the expected result, as a literal */
    const char *output;       /* the program's result, as a literal; NULL
                                 when it gave none that could be read */
    /*
     * What the program printed. stdout keeps its first LC_REPORT_TEXT_MAX
     * bytes and stderr its last ones (starting at a line), so a crash
     * report at the end of stderr survives. Either may hold any bytes.
     */
    const char *stdout_text;
    size_t stdout_len;
    int stdout_truncated;
    const char *stderr_text;
    size_t stderr_len;
    int stderr_truncated;
    long time_ms;             /* CPU time, user plus system */
    long wall_ms;             /* elapsed time */
    long memory_kb;           /* peak resident memory */
    int exit_code;            /* its exit status, or -1 if a signal ended it */
    int signal;               /* the signal that ended it, or 0 */
} lc_case_result;

/* The outcome of a judging run. Release it with lc_report_free. */
typedef struct lc_report {
    /*
     * The verdict of the first case that failed; Accepted when every case
     * ran and passed; LC_VERDICT_NONE when the run was cancelled before
     * any case failed.
     */
    lc_verdict verdict;
    int cancelled;            /* the cancel flag stopped the run */
    size_t total;             /* cases in the run */
    size_t passed;            /* cases accepted */
    const lc_case_result *results; /* the cases that finished, in order */
    size_t nresults;
    long max_time_ms;         /* the largest time_ms of any case */
    long max_memory_kb;       /* the largest memory_kb of any case */
    void *priv_;              /* owned memory; do not touch */
} lc_report;

void lc_report_free(lc_report *report);

/* Progress events, delivered while a run is under way. */
typedef enum {
    LC_EV_CASE_START,         /* a case is about to run */
    LC_EV_CASE_DONE,          /* a case finished */
    LC_EV_DONE                /* the run is over */
} lc_event_kind;

/*
 * What the pointers in an event point to is only valid during the
 * callback; copy what you want to keep.
 */
typedef struct lc_event {
    lc_event_kind kind;
    size_t index;             /* case events: the case's position in the run */
    size_t total;             /* case events: how many cases the run has */
    const lc_case *test_case; /* LC_EV_CASE_START: the case */
    const lc_case_result *result; /* LC_EV_CASE_DONE: how it went */
    const lc_report *report;  /* LC_EV_DONE: the whole run */
} lc_event;

typedef struct lc_hooks {
    /*
     * Called on the judging thread for each event; may be NULL. It must
     * not call back into the same context.
     */
    void (*on_event)(void *ud, const lc_event *ev);
    void *ud;
    /*
     * May be NULL. Setting *cancel to non-zero, from a signal handler or
     * another thread, stops the run within about 50 ms: the program
     * running is killed and the case it was on is dropped from the report.
     */
    const volatile sig_atomic_t *cancel;
} lc_hooks;

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
