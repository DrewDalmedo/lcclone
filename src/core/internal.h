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

/* One setting of the config file (config.c). */
typedef struct lc_config_entry {
    const char *key;        /* static */
    const char *value;      /* in the context's arena */
    int line;
} lc_config_entry;

struct lc_ctx {
    lc_arena arena;         /* lives until lc_close: options, problem index */
    char *opt_problems_dir; /* copy of lc_options.problems_dir, or NULL */
    char *opt_workspace_dir; /* copy of lc_options.workspace_dir, or NULL */
    char *opt_cache_dir;    /* copy of lc_options.cache_dir, or NULL */
    char *opt_config_file;  /* copy of lc_options.config_file, or NULL */
    char *opt_data_dir;     /* copy of lc_options.data_dir, or NULL */
    char *workspace_dir;    /* set by lc_workspace_dir, in `arena` */
    char *cache_dir;        /* set by lc_cache_dir, in `arena` */
    char *data_dir;         /* set by lc_data_dir, in `arena` */

    char *config_file;      /* the config file looked for, or NULL */
    lc_config_entry *config; /* its settings (malloc'ed array) */
    size_t nconfig;
    size_t capconfig;

    lc_progress_entry *progress; /* progress.tsv as last read, by id
                                    (malloc'ed array; strings in `arena`) */
    size_t nprogress;
    size_t capprogress;
    int progress_loaded;    /* progress.tsv has been read */

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
/* Config file (config.c)                                              */
/* ------------------------------------------------------------------ */

/* Reads the config file into ctx->config (lc_open calls it). Problems
 * become warnings; only running out of memory fails. */
int lc_config_read(lc_ctx *ctx);

/* The line of the config file that set `key`, or 0. */
int lc_config_line(const lc_ctx *ctx, const char *key);

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

/*
 * Looks a program up like a shell: a name with a '/' is used as it is,
 * anything else is searched for in $PATH. Returns the path of an
 * executable regular file, in `a`, or NULL.
 */
char *lc_which(lc_arena *a, const char *name);

/* The last part of a path, ignoring trailing slashes, in `a`. */
char *lc_basename(lc_arena *a, const char *path);

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
/* Test cases (cases.c)                                                */
/* ------------------------------------------------------------------ */

/* Parses cases written like a cases.txt of your own ("=> expected" may
 * be left out), from text such as a generator's output. */
int lc_parse_user_cases(lc_ctx *ctx, const char *text, size_t len,
                        const char *filename, lc_cases **out);

/* Parses tests.txt for --regen: like lc_parse_cases, except that a case
 * may leave out "=> expected" (or have nothing after it), and the case
 * after an @gen line may have no inputs yet. */
int lc_parse_regen_cases(lc_ctx *ctx, const char *text, size_t len,
                         const char *filename, lc_cases **out);

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

/*
 * `check:`, such as "param nums" or "prefix nums", against the signature
 * `sig` (NULL when the problem has none, which is an error).
 */
int lc_parse_check(const char *p, size_t n, const lc_sig *sig,
                   lc_result_check *out, lc_str *err);

/* The type a problem's result has under `check` (lc_result_type). */
const lc_type *lc_sig_result_type(const lc_sig *sig,
                                  const lc_result_check *check);

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
    char *const *envp;        /* NULL-terminated environment; NULL keeps
                                 the caller's */
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
    size_t ncustom;           /* the last ncustom cases are your own */
    /* the program is built with sanitizers: no address-space limit and
     * no memory verdict (ASan's shadow memory would trip both), and three
     * times the time limit */
    int sanitize;
    char *const *envp;        /* the programs' environment; NULL: lc's */
    const lc_build *build;    /* becomes the report's `build` */
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

/*
 * The cache directory (lc_options.cache_dir, $XDG_CACHE_HOME/lcclone or
 * ~/.cache/lcclone), or NULL with an error when there is none. It may not
 * exist yet. Valid until lc_close.
 */
const char *lc_cache_dir(lc_ctx *ctx);

/*
 * The data directory for progress and history (lc_options.data_dir,
 * $XDG_DATA_HOME/lcclone or ~/.local/share/lcclone), the same way.
 */
const char *lc_data_dir(lc_ctx *ctx);

/* ------------------------------------------------------------------ */
/* The harness runtime (runtime/, embedded by tools/embed.awk)         */
/* ------------------------------------------------------------------ */

typedef struct lc_rt_file {
    const char *name;         /* its path under runtime/, e.g. "c/lc_rt.c" */
    const char *text;         /* NUL-terminated */
    size_t len;
} lc_rt_file;

extern const lc_rt_file lc_rt_files[];
extern const size_t lc_rt_nfiles;

/* The embedded file with this name; NULL if there is none. */
const lc_rt_file *lc_rt_file_get(const char *name);

/* ------------------------------------------------------------------ */
/* Compilers and standards (lang.c)                                    */
/* ------------------------------------------------------------------ */

typedef struct lc_compiler {
    const char *cmd;          /* the command, e.g. "c++" or "ccache g++" */
    const char *path;         /* its first word, found on $PATH */
    const char *id;           /* cmd, path, size and mtime, on one line */
    const char *flags;        /* the config's cflags or cxxflags, or NULL:
                                 for your solutions only */
} lc_compiler;

/* Finds the language's compiler (the config's cc or cxx, else $CC or
 * $CXX, else cc or c++). Strings live in `a`. LC_ERR_NOTFOUND with a
 * message when it isn't installed. */
int lc_compiler_get(lc_ctx *ctx, const lc_lang *lang, lc_arena *a,
                    lc_compiler *out);

/* A NULL-terminated argv that runs the compiler with these arguments,
 * then `extra` (words for the shell to split; may be NULL), through
 * /bin/sh, in `a`. The arguments are not copied. */
char **lc_compiler_argv(lc_arena *a, const lc_compiler *cc,
                        const char *const *args, size_t nargs,
                        const char *extra);

/*
 * lc_std_resolve for a known compiler: *name is the standard and *flag
 * the spelling this compiler wants after -std= (an older one such as
 * c++2b for c++23 on some compilers). Both are static strings.
 */
int lc_std_pick(lc_ctx *ctx, const lc_lang *lang, const lc_compiler *cc,
                const char *want, const char **name, const char **flag);

/* ------------------------------------------------------------------ */
/* Code generation (codegen.c)                                         */
/* ------------------------------------------------------------------ */

/* LC_OK if lc can build a harness for the problem, else LC_ERR_INVALID
 * with a message saying what isn't supported yet. */
int lc_codegen_check(lc_ctx *ctx, const lc_problem *p);
/*
 * lc_codegen_check without its last test, that the problem has a result:
 * whether lc supports what the signature uses. With it, lc check tells a
 * problem lc can't judge yet from one that returns void and has no
 * check: (a mistake). lc_codegen_void_reason writes what is wrong with
 * that one into buf, LC_VOID_REASON_MAX bytes.
 */
#define LC_VOID_REASON_MAX 160
int lc_codegen_supported(lc_ctx *ctx, const lc_problem *p);
void lc_codegen_void_reason(const lc_problem *p, char *buf);

/* Appends the harness: a main() that reads a case, calls the solution
 * and writes the result, using the runtime (lc_rt_core.h, lc_rt.c(pp)). */
int lc_codegen_harness(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
                       lc_str *out);

/* Appends the #defines that make the prelude declare the node types the
 * problem uses (LC_LISTNODE and the like). */
void lc_codegen_defines(const lc_problem *p, lc_str *out);

/* ------------------------------------------------------------------ */
/* Building (build.c)                                                  */
/* ------------------------------------------------------------------ */

/* Name of the pseudo-file the harness's lines are reported under. */
#define LC_HARNESS_FILE "lc_harness"

/* What a build is of. */
typedef enum {
    LC_ROLE_SOLUTION = 0,     /* your solution: the config's flags apply */
    LC_ROLE_REF,              /* the pack's ref.c or ref.cpp */
    LC_ROLE_BRUTE,            /* the pack's brute.c or brute.cpp */
    LC_ROLE_STUB,             /* the stub lc start writes (lc check) */
    LC_ROLE_GEN               /* the pack's gen.c: lc_gen.h, gen.c and
                                 lc_gen_main.c, not a harness */
} lc_role;

/* "solution", "reference", "brute", "stub" or "generator". */
const char *lc_role_name(int role);

typedef struct lc_build_req {
    const lc_problem *p;
    const lc_lang *lang;
    const lc_compiler *cc;
    const char *src;          /* the source file, an absolute path */
    const char *std;          /* the standard's name and -std= flag, from */
    const char *std_flag;     /* lc_std_pick */
    int sanitize;
    int role;                 /* lc_role */
    const volatile sig_atomic_t *cancel;
} lc_build_req;

/*
 * Fills in the parts of *b known before building (lang, std, source,
 * compiler, sanitize, role), with strings in `a`.
 */
int lc_build_init(lc_ctx *ctx, const lc_build_req *req, lc_arena *a,
                  lc_build *b);

/*
 * Builds the source with its harness into the build cache, unless a
 * build of exactly the same input is there already. Fills in the rest of
 * *b, with strings in `a`, and on success sets *exe to the program's path
 * (in `a`). A compile error is not a failure: b->ok is 0. *cancelled is
 * set when the cancel flag stopped the compiler.
 */
int lc_build_source(lc_ctx *ctx, const lc_build_req *req, lc_arena *a,
                    lc_build *b, const char **exe, int *cancelled);

/*
 * Builds `src` in `lang` for problem `p` as `role`, with build events:
 * finds the compiler, picks the standard (`std` as for lc_std_resolve),
 * and builds. On LC_OK *bout is the build (in `a`) unless *cancelled is
 * set, and *exe the program when it compiled.
 */
int lc_build_step(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
                  const char *src, const char *std, int sanitize, int role,
                  const lc_hooks *hooks, lc_arena *a, lc_build **bout,
                  const char **exe, int *cancelled);

/*
 * Runs `exe`, a solution built with its harness, on the inputs of case
 * `c` in `cwd`, under the problem's limits with twice its time (room for
 * a busy machine, or a slow solution such as brute.c). When it works,
 * *out is its result as a literal, in `a`. When the program fails, *out
 * is NULL, *verdict says how and `why` why. *cancelled is set when the
 * cancel flag stopped it. Fails only when it can't be run.
 */
int lc_run_case(lc_ctx *ctx, const lc_problem *p, const char *exe,
                const char *cwd, const lc_case *c, const lc_hooks *hooks,
                lc_arena *a, const char **out, lc_verdict *verdict,
                lc_str *why, int *cancelled);

/* The exit status of a harness that can't go on (LC_RT_FAIL in
 * runtime/common/lc_rt_core.h). It says why on stderr, in a line that
 * starts with "lc: ". */
#define LC_HARNESS_FAIL 70

/*
 * When a program exited with LC_HARNESS_FAIL, appends the harness's reason
 * from its stderr `err` (NULL is none) to `why`, e.g. "exited with status
 * 70" becomes "exited with status 70: f returned 6, but nums has 5
 * elements". lc check uses it, as its findings show no stderr.
 */
void lc_add_harness_reason(lc_str *why, int exit_code, const char *err);

/* The directory part of a path, in `a`. */
char *lc_dir_of(lc_arena *a, const char *path);

/* The pack's `base`.c or `base`.cpp ("ref", "brute") and its language, in
 * `a`; NULL when it has neither. *both is set when it has both. */
char *lc_pack_source(lc_arena *a, const lc_problem *p, const char *base,
                     const lc_lang **lang, int *both);

/* Finds the compiler's messages in its output (gcc and clang style). */
int lc_parse_diags(lc_arena *a, const char *text, size_t len,
                   const char *source, lc_diag **out, size_t *count);

#endif /* LC_INTERNAL_H */
