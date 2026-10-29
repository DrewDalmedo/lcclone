#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* pager.c: writing the output, directly or through $PAGER. */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cli.h"

int cli_write(const char *data, size_t len)
{
    while (len > 0) {
        ssize_t n = write(STDOUT_FILENO, data, len);

        if (n < 0) {
            if (errno == EINTR)
                continue;
            if (errno == EPIPE)
                return CLI_EXIT_OK; /* e.g. "lc list | head -1" */
            fprintf(stderr, "lc: cannot write the output: %s\n",
                    strerror(errno));
            return CLI_EXIT_ERROR;
        }
        data += n;
        len -= (size_t)n;
    }
    return CLI_EXIT_OK;
}

static int is_blank(int c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

/* 1 if an executable file `name` is in one of the $PATH directories. */
static int on_path(const char *name)
{
    const char *path = getenv("PATH"), *p, *e;
    lc_str buf = LC_STR_INIT;
    struct stat st;
    int found = 0;

    if (!path || !*path)
        path = "/usr/bin:/bin";
    for (p = path; !found; p = e + 1) {
        e = strchr(p, ':');
        if (!e)
            e = p + strlen(p);
        lc_str_clear(&buf);
        if (e == p)
            lc_str_putc(&buf, '.'); /* an empty entry is the current directory */
        else
            lc_str_append(&buf, p, (size_t)(e - p));
        lc_str_putc(&buf, '/');
        lc_str_puts(&buf, name);
        found = !buf.failed && stat(buf.data, &st) == 0 &&
                S_ISREG(st.st_mode) && access(buf.data, X_OK) == 0;
        if (!*e)
            break;
    }
    lc_str_free(&buf);
    return found;
}

/* The pager command to run, or NULL for none. */
static const char *pager_command(void)
{
    const char *pager = getenv("PAGER"), *p;

    if (pager) {
        for (p = pager; is_blank((unsigned char)*p); p++)
            ;
        if (!*p || strcmp(pager, "cat") == 0)
            return NULL;
        return pager;
    }
    if (on_path("less"))
        return "less";
    if (on_path("more"))
        return "more";
    return NULL;
}

/* 1 if the command's program is less ("less", "/usr/bin/less -S", ...). */
static int runs_less(const char *cmd)
{
    const char *start, *end, *base;

    for (start = cmd; is_blank((unsigned char)*start); start++)
        ;
    for (end = start; *end && !is_blank((unsigned char)*end); end++)
        ;
    for (base = end; base > start && base[-1] != '/'; base--)
        ;
    return end - base == 4 && memcmp(base, "less", 4) == 0;
}

int cli_page(const char *data, size_t len, int allow)
{
    const char *cmd;
    FILE *fp;
    size_t written;
    int status;

    if (!allow || !isatty(STDOUT_FILENO))
        return cli_write(data, len);
    cmd = pager_command();
    if (!cmd)
        return cli_write(data, len);
    /* F: quit if it fits on one screen, R: pass colours, X: keep the text
     * on the screen afterwards. */
    if (runs_less(cmd) && !getenv("LESS"))
        setenv("LESS", "FRX", 1);
    /* The pager starts with SIGPIPE at its default, not ignored like here. */
    signal(SIGPIPE, SIG_DFL);
    fp = popen(cmd, "w");
    signal(SIGPIPE, SIG_IGN);
    if (!fp)
        return cli_write(data, len);
    /* The pager owns the terminal now: ^C is for it, and lc must wait for
     * it to finish rather than leave it behind. */
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    /* A short write means the pager quit early, which is fine. */
    written = fwrite(data, 1, len, fp);
    (void)written;
    status = pclose(fp);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    /* The shell's "command not found" or "not executable": print directly. */
    if (status != -1 && WIFEXITED(status) &&
        (WEXITSTATUS(status) == 127 || WEXITSTATUS(status) == 126))
        return cli_write(data, len);
    return CLI_EXIT_OK;
}
