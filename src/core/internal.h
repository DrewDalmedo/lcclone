/*
 * internal.h: helpers shared by the core's source files. Not installed and
 * not for front-ends; they use include/lc.h only.
 *
 * Every core .c file starts with
 *     #define _POSIX_C_SOURCE 200809L
 *     #define _FILE_OFFSET_BITS 64
 * before any #include, so the core sees exactly POSIX.1-2008 plus C11.
 * The second line matters only on 32-bit glibc, where off_t and ino_t are
 * otherwise 32 bits and stat() and readdir() fail with EOVERFLOW on files
 * with large inode numbers (XFS, NFS, overlayfs); it does nothing elsewhere.
 */
#ifndef LC_INTERNAL_H
#define LC_INTERNAL_H

#include <stddef.h>
#include <sys/types.h>

#include "lc.h"

/* Install location of shared data; the Makefile passes the real one. */
#ifndef LC_DATADIR
#define LC_DATADIR "/usr/local/share/lcclone"
#endif

/* ------------------------------------------------------------------ */
/* Arena allocator (arena.c)                                           */
/* ------------------------------------------------------------------ */

/*
 * A bump allocator. Zero-initialize it before use; lc_arena_free releases
 * everything at once and leaves an empty, reusable arena. Every allocation
 * is aligned for any type. Functions return NULL when out of memory or when
 * a size would overflow.
 */
typedef struct lc_arena_block lc_arena_block;
typedef struct lc_arena {
    lc_arena_block *head;
} lc_arena;

void *lc_arena_alloc(lc_arena *a, size_t n);
void *lc_arena_zalloc(lc_arena *a, size_t n);
void *lc_arena_array(lc_arena *a, size_t count, size_t size); /* zeroed */
char *lc_arena_strdup(lc_arena *a, const char *s);
char *lc_arena_strndup(lc_arena *a, const char *s, size_t n);
char *lc_arena_printf(lc_arena *a, const char *fmt, ...) LC_PRINTF(2, 3);
char *lc_arena_vprintf(lc_arena *a, const char *fmt, va_list ap);
void lc_arena_free(lc_arena *a);

/*
 * Grows a malloc'ed array `p` (NULL at first) so it can hold at least `need`
 * elements of size `elem`. Returns the possibly moved array and updates
 * *cap, or returns NULL when out of memory, leaving `p` and *cap untouched:
 *
 *     T *tmp = lc_grow(arr, &cap, n + 1, sizeof *arr);
 *     if (!tmp) return lc_oom(ctx);
 *     arr = tmp;
 */
void *lc_grow(void *p, size_t *cap, size_t need, size_t elem);

/* ------------------------------------------------------------------ */
/* Context internals (ctx.c)                                           */
/* ------------------------------------------------------------------ */

struct lc_ctx {
    lc_arena arena;         /* lives until lc_close: options, problem index */
    char *opt_problems_dir; /* copy of lc_options.problems_dir, or NULL */

    lc_str err;             /* last error message */
    int err_nomem;          /* formatting the last error ran out of memory */

    char **warnings;        /* strings live in `arena` */
    size_t nwarnings;
    size_t capwarnings;

    char *problems_dir;     /* set by lc_load (absolute path), in `arena` */
    lc_problem *problems;   /* sorted by id, in `arena` */
    size_t nproblems;
    int loaded;             /* lc_load has run successfully */

    unsigned long tmp_counter; /* makes temp file names unique (fs.c) */
};

/*
 * Sets the last error message and returns `status`. Arguments may point into
 * the current message (e.g. "%s", lc_last_error(ctx)).
 */
int lc_set_error(lc_ctx *ctx, int status, const char *fmt, ...) LC_PRINTF(3, 4);
/*
 * Like lc_set_error, then appends ": " and the system's message for errnum
 * (from strerror_r, so it is safe with one context per thread).
 */
int lc_set_errno(lc_ctx *ctx, int status, int errnum, const char *fmt, ...)
    LC_PRINTF(4, 5);
/* Sets "out of memory" and returns LC_ERR_NOMEM. */
int lc_oom(lc_ctx *ctx);
/* Records a warning. Returns LC_OK or LC_ERR_NOMEM. */
int lc_add_warning(lc_ctx *ctx, const char *fmt, ...) LC_PRINTF(2, 3);

/* ------------------------------------------------------------------ */
/* Files (fs.c)                                                        */
/* ------------------------------------------------------------------ */

#define LC_MIB ((size_t)1 << 20)

/* Largest files the loader reads whole; anything bigger skips the pack (or
 * its tests) with a warning instead of exhausting memory. */
#define LC_MAX_PROBLEM_MD (16 * LC_MIB)
#define LC_MAX_TESTS_TXT (256 * LC_MIB)

/*
 * Appends the whole file to `out`. Returns LC_ERR_NOTFOUND if it does not
 * exist, LC_ERR_IO for other failures (including a directory, and a file
 * longer than `max` bytes, which is never read past max + 1 bytes), with a
 * message naming the path.
 */
int lc_read_file(lc_ctx *ctx, const char *path, size_t max, lc_str *out);

/*
 * Lists a directory's entries, except "." and "..", sorted by strcmp.
 * The array and names are allocated in `a`.
 */
int lc_list_dir(lc_ctx *ctx, const char *path, lc_arena *a,
                char ***names, size_t *count);

int lc_is_dir(const char *path);  /* 1 if path is a directory (follows links) */
int lc_is_file(const char *path); /* 1 if path is a regular file (follows links) */

/* Joins with one '/'. Returns a copy of `b` when `b` is absolute. */
char *lc_path_join(lc_arena *a, const char *dir, const char *name);

/* realpath(3) into the arena. Returns NULL and sets errno on failure. */
char *lc_realpath(lc_arena *a, const char *path);

/* Creates a directory and its missing parents (like mkdir -p). */
int lc_mkdir_p(lc_ctx *ctx, const char *path, mode_t mode);

/*
 * Writes a file atomically: the data goes to a temporary file in the same
 * directory, which is then renamed over `path`. The new file's mode is
 * 0666 minus the umask.
 */
int lc_write_file_atomic(lc_ctx *ctx, const char *path,
                         const char *data, size_t len);

/* ------------------------------------------------------------------ */
/* Types and signatures (types.c)                                      */
/* ------------------------------------------------------------------ */

/*
 * These parse header values from problem.md. On a bad value they return
 * LC_ERR_PARSE with the message in `err` (cleared first), worded to follow
 * the field name, e.g. "signature: unknown type 'vector<int>'". `what` is
 * that field name. They return LC_ERR_NOMEM when memory runs out.
 */

/* `signature:`, such as "int[] twoSum(int[] nums, int target)". */
int lc_parse_sig(lc_arena *a, const char *p, size_t n, const char *what,
                 lc_sig *out, lc_str *err);

/* A comma-separated parameter list, such as `hidden: int skipA, int skipB`. */
int lc_parse_params(lc_arena *a, const char *p, size_t n, const char *what,
                    lc_param **out, size_t *count, lc_str *err);

/*
 * `compare:`. `result` is the type being compared, or NULL when it isn't
 * known; then only the syntax is checked.
 */
int lc_parse_compare(const char *p, size_t n, const lc_type *result,
                     lc_compare *out, lc_str *err);

/* The default comparison for a result of type `result` (NULL: unknown). */
lc_compare lc_default_compare(const lc_type *result);

/* Deep copies into `a`. They return NULL only when out of memory. */
lc_sig *lc_copy_sig(lc_arena *a, const lc_sig *sig);
lc_param *lc_copy_params(lc_arena *a, const lc_param *params, size_t n);

/* ------------------------------------------------------------------ */
/* Values (value.c, wire.c)                                            */
/* ------------------------------------------------------------------ */

struct lc_pool {
    lc_arena arena;
};

/* Largest output of lc_fmt_double, NUL included. */
#define LC_DOUBLE_BUF 40

/* Deepest nesting of arrays an untyped literal may have. */
#define LC_VALUE_MAX_DEPTH 32

/*
 * Length of the JSON number at the start of p[0..n): -?(0|[1-9][0-9]*)
 * then an optional fraction and exponent. 0 when there is none, or when
 * it is malformed ("01", "1.", "1e", "-"). *is_int is set to 1 when it has
 * no fraction or exponent.
 */
size_t lc_scan_number(const char *p, size_t n, int *is_int);

/*
 * Checks item `idx` of a TreeNode or NaryNode level-order list, given
 * whether it is null and how many items before it weren't. `group` is
 * state for N-ary trees, set when idx is 1. Returns NULL when the item is
 * fine, else what is wrong with it.
 */
const char *lc_tree_item_check(lc_kind node, size_t idx, int is_null,
                               size_t nonnull, size_t *group);

/*
 * Converts p[0..n), which must already be checked to be a decimal number
 * such as "-1.5e+3", whatever the current locale's decimal point is.
 * Returns 0, or -1 when the text is too long or the result overflows.
 */
int lc_strtod(const char *p, size_t n, double *out);

/*
 * The shortest of %.15g, %.16g and %.17g that reads back as `v`, with '.'
 * as the decimal point whatever the locale; "inf", "-inf" or "nan" when
 * `v` isn't finite.
 */
void lc_fmt_double(double v, char *buf);

/* ------------------------------------------------------------------ */
/* Running a program (runner.c)                                        */
/* ------------------------------------------------------------------ */

/* Default cap on each captured stream. */
#define LC_RUN_OUTPUT_MAX LC_MIB

/*
 * What to run and under which limits. Zero-initialize it, then fill in
 * what you need: a zero field means "no limit" or "the default".
 */
typedef struct lc_run_spec {
    const char *path;         /* the program, used as given: no $PATH search */
    char *const *argv;        /* NULL-terminated; NULL means { path, NULL } */
    const char *cwd;          /* directory to run in; NULL keeps the caller's */
    const char *input;        /* written to stdin, which is then closed */
    size_t input_len;
    int result_fd;            /* 1: open fd 3 for the harness's result and
                                 capture it; 0: fd 3 is closed */

    /*
     * Resource limits, set in the child. RLIMIT_CORE is always 0. A limit
     * above the current hard limit is lowered to it, and one that can't be
     * set is skipped: the others still apply.
     */
    int cpu_sec;              /* RLIMIT_CPU: SIGXCPU then, SIGKILL a second later */
    unsigned long long as_bytes;    /* RLIMIT_AS; on macOS, which doesn't
                                       enforce it, the runner itself kills the
                                       program once its memory passes this */
    unsigned long long stack_bytes; /* RLIMIT_STACK */
    unsigned long long fsize_bytes; /* RLIMIT_FSIZE */
    long long wall_ms;        /* killed this long after it started */

    /* Bytes kept of each stream (0: LC_RUN_OUTPUT_MAX). A program that
     * writes more is killed, and lc_run_result.overflow says where. */
    size_t stdout_max;
    size_t stderr_max;
    size_t result_max;

    const volatile sig_atomic_t *cancel; /* non-zero kills it; may be NULL */
} lc_run_spec;

/* lc_run_result.overflow bits. */
enum { LC_RUN_STDOUT = 1, LC_RUN_STDERR = 2, LC_RUN_RESULT = 4 };

/* What happened to a program. Initialize with lc_run_result_init; reuse it
 * for several runs, then release it with lc_run_result_free. */
typedef struct lc_run_result {
    int exited;               /* 1 if it exited, with exit_code */
    int exit_code;
    int signal;               /* the signal that ended it, else 0 */
    int timed_out;            /* killed at the wall-clock deadline */
    int cancelled;            /* killed because *cancel was set */
    int mem_killed;           /* killed for passing as_bytes (macOS only) */
    unsigned overflow;        /* LC_RUN_* streams that passed their cap */
    long cpu_ms;              /* user plus system CPU time */
    long wall_ms;
    long max_rss_kb;          /* peak resident memory */
    lc_str out;               /* stdout */
    lc_str err;               /* stderr */
    lc_str result;            /* fd 3 */
} lc_run_result;

void lc_run_result_init(lc_run_result *r);
void lc_run_result_free(lc_run_result *r);

/*
 * Runs a program to completion: forks, sets up the child (own process
 * group, signals reset to their defaults, only fds 0-3 open, limits, cwd),
 * execs it, writes its input and reads its output in one poll() loop, and
 * waits for it by PID. Whatever the program does, that is reported in *r
 * and lc_run returns LC_OK. It fails only when the program can't be
 * started (LC_ERR_NOTFOUND for a missing program or directory, else
 * LC_ERR_IO) or the system lets it down (LC_ERR_IO, LC_ERR_NOMEM).
 *
 * The caller's process must ignore SIGPIPE and not ignore SIGCHLD (see
 * "Judging" in lc.h).
 */
int lc_run(lc_ctx *ctx, const lc_run_spec *spec, lc_run_result *r);

/* ------------------------------------------------------------------ */
/* Judging a built program (judge.c)                                   */
/* ------------------------------------------------------------------ */

/* Largest result a harness may write on fd 3. */
#define LC_RESULT_MAX (16 * LC_MIB)

/* The limits a case is judged by (docs/judging.md, "Limits"). */
typedef struct lc_limits {
    int time_ms;              /* CPU time; Time Limit Exceeded beyond it */
    int memory_mb;            /* peak resident memory; Memory Limit
                                 Exceeded beyond it */
    size_t output_max;        /* stdout and stderr, each */
    size_t result_max;        /* the result on fd 3 */
} lc_limits;

/* Limits with the given time and memory, and the default output caps. */
void lc_limits_init(lc_limits *l, int time_ms, int memory_mb);

/* Sets the rlimits, deadline and caps of `spec` for one case. */
void lc_limits_apply(const lc_limits *l, lc_run_spec *spec);

/*
 * The verdict a run earns before its result is looked at: Output, Memory
 * or Time Limit Exceeded, or Runtime Error, in that order of precedence,
 * with the reason in `why`. LC_VERDICT_NONE when the program exited 0
 * within every limit, so its result decides. Not for cancelled runs.
 */
lc_verdict lc_classify(const lc_limits *l, const lc_run_result *r,
                       lc_str *why);

typedef struct lc_judge_opts {
    const char *cwd;          /* directory to run in; NULL keeps the caller's */
    int stop_on_failure;      /* stop after the first case not accepted */
    int time_ms;              /* 0: the problem's time_limit_ms */
    int memory_mb;            /* 0: the problem's memory_limit_mb */
} lc_judge_opts;

/*
 * Judges the program `exe`, a solution already built with its harness,
 * against `cases` of problem `p`: for each case it encodes the inputs
 * (docs/wire.md), runs the program, classifies the run, decodes and
 * compares the result, and reports events through `hooks` (may be NULL).
 * `opts` may be NULL. On LC_OK *out holds the report.
 *
 * Fails with LC_ERR_INVALID for a problem that can't be judged yet (no
 * signature, a void result, compare: checker) or no cases, with
 * LC_ERR_PARSE for a case that doesn't fit the signature, and as lc_run
 * does when the program can't be run.
 */
int lc_judge_exe(lc_ctx *ctx, const lc_problem *p, const char *exe,
                 const lc_case *cases, size_t ncases,
                 const lc_judge_opts *opts, const lc_hooks *hooks,
                 lc_report **out);

/* getenv, but NULL when the variable is unset or empty. */
const char *lc_getenv(const char *name);

/*
 * An XDG base directory: $<env> if it is set to an absolute path, else
 * $HOME/<fallback>. NULL when neither is usable.
 * Example: lc_xdg_dir(a, "XDG_DATA_HOME", ".local/share").
 */
char *lc_xdg_dir(lc_arena *a, const char *env, const char *fallback);

#endif /* LC_INTERNAL_H */
