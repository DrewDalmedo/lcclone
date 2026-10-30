/* realpath() is only declared for the XSI level on glibc. */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "internal.h"

static int not_found_errno(int e)
{
    return e == ENOENT || e == ENOTDIR;
}

int lc_read_file(lc_ctx *ctx, const char *path, lc_str *out)
{
    struct stat st;
    int fd;

    do
        fd = open(path, O_RDONLY | O_CLOEXEC);
    while (fd < 0 && errno == EINTR);
    if (fd < 0)
        return lc_set_errno(ctx, not_found_errno(errno) ? LC_ERR_NOTFOUND
                                                        : LC_ERR_IO,
                            errno, "cannot open '%s'", path);
    if (fstat(fd, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            close(fd);
            return lc_set_errno(ctx, LC_ERR_IO, EISDIR, "cannot read '%s'", path);
        }
        if (S_ISREG(st.st_mode) && st.st_size > 0 &&
            (unsigned long long)st.st_size < (unsigned long long)SIZE_MAX) {
            if (lc_str_reserve(out, (size_t)st.st_size) != LC_OK) {
                close(fd);
                return lc_oom(ctx);
            }
        }
    }
    for (;;) {
        ssize_t n;

        if (lc_str_reserve(out, 4096) != LC_OK) {
            close(fd);
            return lc_oom(ctx);
        }
        n = read(fd, out->data + out->len, out->cap - out->len - 1);
        if (n < 0) {
            int e = errno;
            if (e == EINTR)
                continue;
            close(fd);
            return lc_set_errno(ctx, LC_ERR_IO, e, "cannot read '%s'", path);
        }
        if (n == 0)
            break;
        out->len += (size_t)n;
        out->data[out->len] = '\0';
    }
    close(fd);
    return LC_OK;
}

static int cmp_names(const void *a, const void *b)
{
    return strcmp(*(char *const *)a, *(char *const *)b);
}

int lc_list_dir(lc_ctx *ctx, const char *path, lc_arena *a,
                char ***names, size_t *count)
{
    DIR *d;
    char **list = NULL, **out;
    size_t n = 0, cap = 0, i;
    int rc = LC_OK;

    *names = NULL;
    *count = 0;
    d = opendir(path);
    if (!d)
        return lc_set_errno(ctx, not_found_errno(errno) ? LC_ERR_NOTFOUND
                                                        : LC_ERR_IO,
                            errno, "cannot open directory '%s'", path);
    for (;;) {
        struct dirent *e;
        char *name, **tmp;

        errno = 0;
        e = readdir(d);
        if (!e) {
            if (errno) {
                rc = lc_set_errno(ctx, LC_ERR_IO, errno,
                                  "cannot read directory '%s'", path);
                goto done;
            }
            break;
        }
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;
        tmp = lc_grow(list, &cap, n + 1, sizeof *list);
        if (!tmp) {
            rc = lc_oom(ctx);
            goto done;
        }
        list = tmp;
        name = lc_arena_strdup(a, e->d_name);
        if (!name) {
            rc = lc_oom(ctx);
            goto done;
        }
        list[n++] = name;
    }
    if (n)
        qsort(list, n, sizeof *list, cmp_names);
    out = lc_arena_array(a, n ? n : 1, sizeof *out);
    if (!out) {
        rc = lc_oom(ctx);
        goto done;
    }
    for (i = 0; i < n; i++)
        out[i] = list[i];
    *names = out;
    *count = n;
done:
    free(list);
    closedir(d);
    return rc;
}

int lc_is_dir(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

int lc_is_file(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

char *lc_path_join(lc_arena *a, const char *dir, const char *name)
{
    size_t dl;

    if (name[0] == '/' || dir[0] == '\0')
        return lc_arena_strdup(a, name);
    dl = strlen(dir);
    while (dl > 1 && dir[dl - 1] == '/')
        dl--;
    if (dl == 1 && dir[0] == '/')
        return lc_arena_printf(a, "/%s", name);
    return lc_arena_printf(a, "%.*s/%s", (int)dl, dir, name);
}

char *lc_realpath(lc_arena *a, const char *path)
{
    char *r = realpath(path, NULL);
    char *copy;

    if (!r)
        return NULL;
    copy = lc_arena_strdup(a, r);
    free(r);
    if (!copy)
        errno = ENOMEM;
    return copy;
}

int lc_mkdir_p(lc_ctx *ctx, const char *path, mode_t mode)
{
    lc_str buf = LC_STR_INIT;
    size_t i;
    int rc = LC_OK;

    if (lc_str_puts(&buf, path) != LC_OK)
        return lc_oom(ctx);
    /* Strip trailing slashes, but keep a lone "/". */
    while (buf.len > 1 && buf.data[buf.len - 1] == '/')
        buf.data[--buf.len] = '\0';
    for (i = 1; i <= buf.len; i++) {
        if (i < buf.len && buf.data[i] != '/')
            continue;
        if (i < buf.len && buf.data[i - 1] == '/')
            continue; /* repeated slash */
        buf.data[i] = '\0';
        if (mkdir(buf.data, mode) != 0) {
            int e = errno;
            /* Some systems report EACCES or EROFS rather than EEXIST for a
             * directory that already exists, so check for one directly. */
            if (!lc_is_dir(buf.data)) {
                rc = lc_set_errno(ctx, LC_ERR_IO, e == EEXIST ? ENOTDIR : e,
                                  "cannot create directory '%s'", buf.data);
                break;
            }
        }
        if (i < buf.len)
            buf.data[i] = '/';
    }
    lc_str_free(&buf);
    return rc;
}

static int write_all(int fd, const char *data, size_t len)
{
    while (len > 0) {
        ssize_t n = write(fd, data, len);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        data += n;
        len -= (size_t)n;
    }
    return 0;
}

int lc_write_file_atomic(lc_ctx *ctx, const char *path,
                         const char *data, size_t len)
{
    lc_str tmp = LC_STR_INIT;
    int fd = -1, tries, e;

    for (tries = 0; tries < 100; tries++) {
        lc_str_clear(&tmp);
        if (lc_str_printf(&tmp, "%s.tmp-%ld-%lu", path, (long)getpid(),
                          ++ctx->tmp_counter) != LC_OK) {
            lc_str_free(&tmp);
            return lc_oom(ctx);
        }
        fd = open(tmp.data, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0666);
        if (fd >= 0 || errno != EEXIST)
            break;
    }
    if (fd < 0) {
        e = errno;
        lc_str_free(&tmp);
        return lc_set_errno(ctx, LC_ERR_IO, e, "cannot write '%s'", path);
    }
    if (write_all(fd, data, len) != 0 || fsync(fd) != 0) {
        e = errno;
        close(fd);
        unlink(tmp.data);
        lc_str_free(&tmp);
        return lc_set_errno(ctx, LC_ERR_IO, e, "cannot write '%s'", path);
    }
    if (close(fd) != 0) {
        e = errno;
        unlink(tmp.data);
        lc_str_free(&tmp);
        return lc_set_errno(ctx, LC_ERR_IO, e, "cannot write '%s'", path);
    }
    if (rename(tmp.data, path) != 0) {
        e = errno;
        unlink(tmp.data);
        lc_str_free(&tmp);
        return lc_set_errno(ctx, LC_ERR_IO, e, "cannot replace '%s'", path);
    }
    lc_str_free(&tmp);
    return LC_OK;
}

const char *lc_getenv(const char *name)
{
    const char *v = getenv(name);
    return v && *v ? v : NULL;
}

char *lc_xdg_dir(lc_arena *a, const char *env, const char *fallback)
{
    const char *v = lc_getenv(env);
    const char *home;

    if (v && v[0] == '/')
        return lc_arena_strdup(a, v);
    home = lc_getenv("HOME");
    if (!home)
        return NULL;
    return lc_path_join(a, home, fallback);
}
