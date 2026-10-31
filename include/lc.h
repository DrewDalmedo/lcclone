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
    /*
     * Directory for your solutions, one NNNN-slug directory per problem.
     * When NULL: $LC_WORKSPACE, else the config's workspace, else ~/lc.
     */
    const char *workspace_dir;
    /*
     * Directory for builds and other things lc can recreate. When NULL:
     * $XDG_CACHE_HOME/lcclone, else ~/.cache/lcclone.
     */
    const char *cache_dir;
    /*
     * The config file. When NULL: $LC_CONFIG, else
     * $XDG_CONFIG_HOME/lcclone/config, else ~/.config/lcclone/config.
     * "" means no config file at all.
     */
    const char *config_file;
    /*
     * Directory for your progress and submission history. When NULL:
     * $XDG_DATA_HOME/lcclone, else ~/.local/share/lcclone.
     */
    const char *data_dir;
} lc_options;

void lc_options_init(lc_options *opt);

/*
 * Creates a context and reads the config file (see "Configuration"
 * below). `opt` may be NULL. Returns NULL only when out of memory.
 */
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
/* Configuration (docs/workspace.md, "Configuration")                  */
/* ------------------------------------------------------------------ */

/*
 * The config file holds `key = value` lines. The keys are cc, cxx,
 * cstd, cxxstd, cflags, cxxflags, workspace (used by the core), and
 * editor, lang, color and open_statement (for front-ends). lc_open reads
 * it; a line that can't be used, such as an unknown key or a bad value,
 * is skipped with a warning. A missing file at the default location is
 * no config; one named by lc_options.config_file or $LC_CONFIG that
 * can't be read gives a warning.
 *
 * Where a setting can come from several places, the most specific wins:
 * a command-line option, then an lc variable ($LC_CXXSTD, $LC_WORKSPACE),
 * then the config file, then a general variable ($CXX, $VISUAL, $EDITOR,
 * $NO_COLOR), then the default.
 */

/* The config file lc_open looked for, or NULL when there was none to
 * look for (no HOME, or config_file ""). It may not exist. */
const char *lc_config_file(const lc_ctx *ctx);

/* A key's value, or NULL when it is unset. A `workspace` starting with
 * "~/" has HOME put in. */
const char *lc_config_get(const lc_ctx *ctx, const char *key);

/* 1 for yes, true, on and 1; 0 for no, false, off and 0; else -1. */
int lc_config_bool(const char *value);

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
    /* literal after "=>"; NULL for a case of your own without one, whose
     * expected result comes from the reference solution */
    const char *expected;
    const char *explanation;  /* text after "@example", or NULL */
    int example;              /* 1 if marked with @example */
    /* the arguments of its "@gen" line, such as "7" or "7 small", when the
     * pack's gen.c made it (docs/authoring.md), or NULL */
    const char *gen;
    int line;                 /* line of the case's first input (of its
                                 @gen line when it has no input yet) */
    int last_line;            /* line of its last input; 0 when none */
    int expected_line;        /* line of its "=>"; 0 when none */
    const char *file;         /* the file it was read from, as given */
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

/*
 * Reads a file of your own cases, such as a workspace's cases.txt. It is
 * written like tests.txt, except that the "=> expected" line may be left
 * out: a blank line or the end of the file then ends the case, and its
 * expected result comes from the problem's reference solution. @example
 * is not allowed. A missing file is an error.
 */
int lc_load_user_cases(lc_ctx *ctx, const char *path, lc_cases **out);

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
    int line;                 /* line of the case in its file */
    const char *file;         /* that file: the pack's tests.txt, or yours */
    int custom;               /* 1 for a case of your own */
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

/* One message of the compiler, such as an error or a warning. */
typedef struct lc_diag {
    const char *file;         /* as the compiler printed it */
    int line;                 /* 0 when the compiler gave none */
    int column;               /* 0 when the compiler gave none */
    const char *severity;     /* "error", "warning" or "note" */
    const char *message;
    int in_solution;          /* 1 if `file` is the source being built */
} lc_diag;

/* How a source file was built. Strings are owned by what holds it. */
typedef struct lc_build {
    /* 1 for the problem's reference solution, which is built to work out
     * the expected results of cases that don't give one */
    int reference;
    /* what is built: "solution" (yours), "reference", and for lc check
     * "brute", "stub" and "generator" */
    const char *role;
    const char *lang;         /* lc_lang.name: "c" or "cpp" */
    const char *std;          /* the standard used, e.g. "c++17" */
    const char *source;       /* the source file, an absolute path */
    const char *compiler;     /* the compiler command, e.g. "c++" */
    int sanitize;             /* built with the sanitizers */
    int ok;                   /* it compiled */
    int cached;               /* an earlier build of the same input was used */
    long time_ms;             /* how long compiling took; 0 when cached */
    /*
     * What the compiler printed, stdout then stderr; when cached, what it
     * printed when the program was built. Paths in it are absolute.
     */
    const char *output;
    size_t output_len;
    const lc_diag *diags;     /* the messages found in `output` */
    size_t ndiags;
    /*
     * The function the harness calls, as lc_prototype gives it in this
     * build's language; NULL for lc check's generator, which has no
     * harness. Set from LC_EV_BUILD_START on.
     */
    const char *prototype;
} lc_build;

/* The outcome of a judging run. Release it with lc_report_free. */
typedef struct lc_report {
    /*
     * The verdict of the first case that failed; Accepted when every case
     * ran and passed; Compile Error when the solution didn't compile;
     * LC_VERDICT_NONE when the run was cancelled before any case failed.
     */
    lc_verdict verdict;
    int cancelled;            /* the cancel flag stopped the run */
    size_t total;             /* cases in the run */
    size_t passed;            /* cases accepted */
    const lc_case_result *results; /* the cases that finished, in order */
    size_t nresults;
    long max_time_ms;         /* the largest time_ms of any case */
    long max_memory_kb;       /* the largest memory_kb of any case */
    const lc_build *build;    /* lc_judge: how the solution was built */
    void *priv_;              /* owned memory; do not touch */
} lc_report;

void lc_report_free(lc_report *report);

/* Progress events, delivered while a run is under way. */
typedef enum {
    LC_EV_CASE_START,         /* a case is about to run */
    LC_EV_CASE_DONE,          /* a case finished */
    LC_EV_DONE,               /* the run is over */
    LC_EV_BUILD_START,        /* lc_judge: a source is about to be built */
    LC_EV_BUILD_DONE,         /* lc_judge: it was built, or failed to */
    LC_EV_FINDING             /* lc_check: something was found */
} lc_event_kind;

typedef struct lc_finding lc_finding;

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
    /* build events: what is built; at LC_EV_BUILD_START only `reference`,
     * `role`, `lang`, `std`, `source`, `compiler`, `sanitize` and
     * `prototype` are set */
    const lc_build *build;
    const lc_finding *finding; /* LC_EV_FINDING */
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
/* Languages                                                           */
/* ------------------------------------------------------------------ */

typedef enum { LC_LANG_C = 0, LC_LANG_CPP } lc_lang_id;

/* A solution language. The table is fixed; pointers stay valid. */
typedef struct lc_lang {
    lc_lang_id id;
    const char *name;         /* "c", "cpp": for --lang and JSON */
    const char *title;        /* "C", "C++" */
    const char *ext;          /* extension of a solution file: "c", "cpp" */
    const char *compiler_env; /* variable naming the compiler: "CC", "CXX" */
    const char *compiler;     /* the compiler without it: "cc", "c++" */
    const char *std_env;      /* variable naming the standard: "LC_CSTD",
                                 "LC_CXXSTD" */
    const char *default_std;  /* "c11", "c++11" */
    const char *const *stds;  /* the standards it accepts, oldest first,
                                 NULL-terminated; "max" is accepted too */
} lc_lang;

size_t lc_lang_count(void);
const lc_lang *lc_lang_at(size_t i);
/* By name: "c", "cpp", or "c++" and "cxx" for C++. NULL if unknown. */
const lc_lang *lc_lang_find(const char *name);
/* By a file's extension: .c is C; .cpp, .cc and .cxx are C++. */
const lc_lang *lc_lang_for_path(const char *path);

/*
 * 1 if the language's compiler can be found: the config's cc or cxx, else
 * $CC or $CXX if set (its first word, looked up on $PATH), else cc or c++.
 */
int lc_lang_installed(lc_ctx *ctx, const lc_lang *lang);

/*
 * The standard a build would use: `want` if not NULL (e.g. from --std),
 * else $LC_CSTD or $LC_CXXSTD, else the config's cstd or cxxstd, else the
 * default (c11, c++11). "max" means
 * the newest standard the compiler accepts. A standard other than the
 * default is checked once against the compiler, and the answer is kept in
 * the cache directory. On LC_OK *out is the standard's name, valid until
 * lc_close.
 */
int lc_std_resolve(lc_ctx *ctx, const lc_lang *lang, const char *want,
                   const char **out);

/*
 * Appends the code a solution starts from, in LeetCode's style for the
 * language: a function in C, a Solution class in C++.
 */
int lc_stub(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
            lc_str *out);

/*
 * Appends the solution's declaration as lc's harness calls it, e.g.
 * "int* twoSum(int* nums, int numsSize, int target, int* returnSize)" in C
 * or "vector<int> Solution::twoSum(vector<int>& nums, int target)" in C++.
 */
int lc_prototype(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
                 lc_str *out);

/* ------------------------------------------------------------------ */
/* Judging a solution                                                  */
/* ------------------------------------------------------------------ */

typedef struct lc_judge_options {
    const char *std;          /* the standard, as for lc_std_resolve */
    int sanitize;             /* build with AddressSanitizer and UBSan */
    int examples;             /* run the pack's @example cases */
    int all;                  /* run every case of the pack (overrides
                                 `examples`) */
    const lc_cases *cases;    /* cases of your own to run after those, or
                                 NULL; see lc_load_user_cases */
    int stop_on_failure;      /* stop after the first case not accepted */
} lc_judge_options;

/*
 * Builds the solution in `src` (an existing file) for problem `p` in
 * `lang`, then runs it against the cases `opts` asks for and returns the
 * report. Events report the build and each case. Your cases without an
 * expected result get it from the pack's reference solution (ref.c or
 * ref.cpp), which is built first.
 *
 * A solution that doesn't compile is not a failure of lc_judge: it returns
 * LC_OK and a report whose verdict is Compile Error, with no results.
 * lc_judge fails for a problem lc can't judge yet, a case that doesn't fit
 * the signature, a compiler that can't be run, an unknown or unsupported
 * standard, and a reference solution that is missing, doesn't compile or
 * fails on a case.
 */
int lc_judge(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
             const char *src, const lc_judge_options *opts,
             const lc_hooks *hooks, lc_report **out);

/* ------------------------------------------------------------------ */
/* Workspace (docs/workspace.md)                                       */
/* ------------------------------------------------------------------ */

/* The workspace directory (see lc_options.workspace_dir), or NULL with an
 * error when there is no HOME to put it in. Valid until lc_close. */
const char *lc_workspace_dir(lc_ctx *ctx);

/* <workspace>/NNNN-slug, the problem's directory there. */
int lc_problem_workspace(lc_ctx *ctx, const lc_problem *p, lc_str *out);

/* <workspace>/NNNN-slug/solution.<ext>. */
int lc_solution_path(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
                     lc_str *out);

/* What lc_start did. Release it with lc_start_info_free. */
typedef struct lc_start_info {
    lc_str dir;               /* the problem's directory */
    lc_str solution;          /* the solution file */
    lc_str readme;            /* README.md */
    lc_str cases;             /* cases.txt */
    int created_solution;     /* 1 if the solution file is new */
    int created_cases;        /* 1 if cases.txt is new */
} lc_start_info;

void lc_start_info_free(lc_start_info *info);

/*
 * Sets up the problem's directory in the workspace: README.md with the
 * statement and examples (rewritten every time), the solution file with
 * the stub and cases.txt (both only if missing: they are yours), and at
 * the top of the workspace a .clangd file and copies of the preludes it
 * points at, so that clangd understands solution files. A .clangd that lc
 * didn't write is left alone, with a warning.
 */
int lc_start(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
             lc_start_info *out);

/* Appends the README.md text of a problem: its title, statement and
 * examples as markdown. `cases` (may be NULL) supplies the examples. */
int lc_readme(lc_ctx *ctx, const lc_problem *p, const lc_cases *cases,
              lc_str *out);

/* ------------------------------------------------------------------ */
/* Progress and history (docs/workspace.md, "Progress and history")    */
/* ------------------------------------------------------------------ */

typedef enum {
    LC_TODO = 0,              /* never submitted */
    LC_ATTEMPTED,             /* submitted, never accepted */
    LC_SOLVED                 /* accepted at least once */
} lc_status;

/* "todo", "attempted" or "solved". */
const char *lc_status_name(lc_status s);
/* Parses those names; -1 for anything else. */
int lc_status_from_name(const char *s);

/* Your progress on one problem. */
typedef struct lc_progress_entry {
    int id;
    lc_status status;
    /* the language of the last accepted submission, else of the last
     * one: "c" or "cpp"; "" when never submitted */
    const char *lang;
    /* the slowest case's CPU time in the best accepted submission built
     * without sanitizers; -1 when there is none */
    long best_ms;
    long long last_ts;        /* last submitted, in seconds since the
                                 epoch; 0 when never */
} lc_progress_entry;

/*
 * Your progress on problem `id`, from progress.tsv in the data directory
 * (lc_options.data_dir). The file is read on first use; a line that
 * can't be used is skipped with a warning, and so is a file that can't
 * be read (then nothing has been submitted). A problem never submitted is
 * LC_TODO with lang "", best_ms -1 and last_ts 0. Fails only when memory
 * runs out. Strings stay valid until lc_close.
 */
int lc_progress(lc_ctx *ctx, int id, lc_progress_entry *out);

/* What lc_record did. Release it with lc_record_info_free. */
typedef struct lc_record_info {
    int recorded;             /* progress.tsv was updated */
    lc_progress_entry before; /* the progress before this submission */
    lc_progress_entry after;  /* and after it (set when recorded) */
    lc_str saved;             /* the copy kept in the history; empty if
                                 none could be made */
} lc_record_info;

void lc_record_info_free(lc_record_info *info);

/*
 * Records a submission: `src`, in `lang`, for problem `p`, judged with
 * every case and got `report` (anything but a cancelled run). progress.tsv
 * is read again, under a lock, so that two lc processes can record at
 * once, updated and replaced atomically. Then the solution is copied to
 * <data>/submissions/NNNN/<time>-<verdict>.<ext>, where <time> is UTC,
 * e.g. 20261030T201503Z. When only that copy fails, the progress is still
 * recorded: lc_record returns the error and info->saved is empty.
 * `sanitize` says the solution was built with the sanitizers, whose times
 * don't count for best_ms. info->recorded says whether progress.tsv was
 * updated, whatever lc_record returns.
 */
int lc_record(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
              const char *src, const lc_report *report, int sanitize,
              lc_record_info *info);

/* How many problems there are, and how many you solved and attempted. */
typedef struct lc_stats_row {
    const char *name;         /* a difficulty or tag name; "all" */
    size_t total;
    size_t solved;
    size_t attempted;
} lc_stats_row;

typedef struct lc_stats_summary {
    lc_stats_row all;
    lc_stats_row difficulty[4]; /* by lc_difficulty: [LC_EASY] and so on */
    const lc_stats_row *tags; /* every tag of the loaded problems, by name */
    size_t ntags;
    void *priv_;              /* owned memory; do not touch */
} lc_stats_summary;

/* Counts the loaded problems against your progress. Release the summary
 * with lc_stats_free. */
int lc_stats(lc_ctx *ctx, lc_stats_summary *out);
void lc_stats_free(lc_stats_summary *s);

/* ------------------------------------------------------------------ */
/* The manifest (docs/authoring.md, "MANIFEST.tsv")                    */
/* ------------------------------------------------------------------ */

/* One row of MANIFEST.tsv: a LeetCode problem and how far its pack is. */
typedef struct lc_manifest_row {
    int id;
    const char *slug;
    const char *title;
    lc_difficulty difficulty;
    int premium;              /* 1 for a premium-only problem */
    const char *group;        /* the harness group, such as "plain" or
                                 "tree"; "skip" when out of scope */
    const char *skip;         /* why it is out of scope (the text after
                                 "skip:"), or NULL */
    const char *status;       /* "todo", "draft" or "verified" */
    int line;                 /* its line in MANIFEST.tsv */
} lc_manifest_row;

typedef struct lc_manifest {
    const char *path;         /* MANIFEST.tsv, an absolute path */
    const lc_manifest_row *rows; /* by id */
    size_t count;
    void *priv_;              /* owned memory; do not touch */
} lc_manifest;

/*
 * Reads MANIFEST.tsv in the problem directory (lc_load must have run).
 * Sets *out to NULL, and returns LC_OK, when there is none. A malformed
 * file gives LC_ERR_PARSE and "<path>:<line>: <message>".
 */
int lc_load_manifest(lc_ctx *ctx, lc_manifest **out);
/* Parses MANIFEST.tsv text; `path` is used in messages. */
int lc_parse_manifest(lc_ctx *ctx, const char *text, size_t len,
                      const char *path, lc_manifest **out);
void lc_manifest_free(lc_manifest *m);
/* The row for problem `id`, or NULL. */
const lc_manifest_row *lc_manifest_find(const lc_manifest *m, int id);

/* ------------------------------------------------------------------ */
/* Checking problem packs (docs/authoring.md, "lc check")              */
/* ------------------------------------------------------------------ */

typedef enum {
    LC_FINDING_ERROR = 0,     /* the pack is wrong; the check fails */
    LC_FINDING_WARNING,       /* probably wrong, or not checked */
    LC_FINDING_NOTE           /* for information */
} lc_finding_level;

/* "error", "warning" or "note". */
const char *lc_finding_level_name(lc_finding_level level);

/* Something lc_check found. Strings are owned by the report. */
struct lc_finding {
    lc_finding_level level;
    const char *file;         /* the file it is about (an absolute path),
                                 or NULL for the pack as a whole */
    int line;                 /* its line in `file`, or 0 */
    const char *message;      /* one line */
    const char *detail;       /* more, as lines ending in '\n': a case's
                                 input and results, or a compiler's output;
                                 NULL when there is none */
};

/* Random cases lc_check compares ref and brute on, by default. */
#define LC_CHECK_COUNT 300

typedef struct lc_check_options {
    /* first rewrite tests.txt: make the cases marked @gen with gen.c,
     * and the expected results with ref (keeping those that already
     * match) */
    int regen;
    unsigned long count;      /* random cases for the brute cross-check;
                                 0 means LC_CHECK_COUNT */
    unsigned long long seed;  /* the cross-check's first seed (lc check
                                 starts at 1); seeds wrap around at 2^64 */
} lc_check_options;

/* What lc_check found and did. Release it with lc_check_report_free. */
typedef struct lc_check_report {
    int ok;                   /* no errors */
    int cancelled;            /* the cancel flag stopped it */
    /* why the pack's solutions were neither built nor run (the problem
     * needs something lc can't judge yet), or NULL when they were */
    const char *skipped;
    size_t errors;
    size_t warnings;
    const lc_finding *findings; /* in the order they were found */
    size_t nfindings;
    size_t ncases;            /* in tests.txt */
    size_t nexamples;         /* of them, marked @example */
    size_t ngenerated;        /* of them, marked @gen */
    const char *ref;          /* the reference solution's file name, e.g.
                                 "ref.c", or NULL */
    const char *brute;        /* brute.c or brute.cpp, or NULL */
    const char *gen;          /* "gen.c", or NULL */
    size_t stubs;             /* languages whose stub was built */
    size_t ref_passed;        /* cases of tests.txt the reference passed */
    long ref_max_ms;          /* its slowest case's CPU time */
    size_t cross;             /* random cases ref and brute agreed on */
    size_t regen_changed;     /* --regen: expected results written */
    size_t regen_generated;   /* --regen: cases made with gen.c */
    int regen_written;        /* --regen: tests.txt was rewritten */
    void *priv_;              /* owned memory; do not touch */
} lc_check_report;

/*
 * Checks problem p's pack, for authors: it agrees with MANIFEST.tsv (`m`,
 * may be NULL); tests.txt reads and every case fits the signature; the
 * stubs build with lc's harness in C and C++; the reference solution
 * passes every case; the cases marked @gen are what gen.c makes; and,
 * with brute.c and gen.c, ref and brute agree on opts->count small random
 * cases. With opts->regen, tests.txt is rewritten first. A problem lc
 * can't judge yet still has its tests.txt checked.
 *
 * Findings arrive as LC_EV_FINDING events as they are found, with build
 * events; the runs themselves send none. What is wrong with the pack is a
 * finding, not a failure: lc_check fails only when out of memory or when
 * the cache can't be written.
 */
int lc_check(lc_ctx *ctx, const lc_problem *p, const lc_manifest *m,
             const lc_check_options *opts, const lc_hooks *hooks,
             lc_check_report **out);
void lc_check_report_free(lc_check_report *report);

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
