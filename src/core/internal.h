/*
 * internal.h: helpers shared by the core's source files. Not installed and
 * not for front-ends; they use include/lc.h only.
 *
 * Every core .c file starts with
 *     #define _POSIX_C_SOURCE 200809L
 * before any #include, so the core sees exactly POSIX.1-2008 plus C11.
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
/* Like lc_set_error, then appends ": " and strerror(errnum). */
int lc_set_errno(lc_ctx *ctx, int status, int errnum, const char *fmt, ...)
    LC_PRINTF(4, 5);
/* Sets "out of memory" and returns LC_ERR_NOMEM. */
int lc_oom(lc_ctx *ctx);
/* Records a warning. Returns LC_OK or LC_ERR_NOMEM. */
int lc_add_warning(lc_ctx *ctx, const char *fmt, ...) LC_PRINTF(2, 3);

/* ------------------------------------------------------------------ */
/* Files (fs.c)                                                        */
/* ------------------------------------------------------------------ */

/*
 * Appends the whole file to `out`. Returns LC_ERR_NOTFOUND if it does not
 * exist, LC_ERR_IO for other failures (including a directory), with a
 * message naming the path.
 */
int lc_read_file(lc_ctx *ctx, const char *path, lc_str *out);

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

/* getenv, but NULL when the variable is unset or empty. */
const char *lc_getenv(const char *name);

/*
 * An XDG base directory: $<env> if it is set to an absolute path, else
 * $HOME/<fallback>. NULL when neither is usable.
 * Example: lc_xdg_dir(a, "XDG_DATA_HOME", ".local/share").
 */
char *lc_xdg_dir(lc_arena *a, const char *env, const char *fallback);

#endif /* LC_INTERNAL_H */
