/* realpath() is only declared for the XSI level on glibc. */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

/*
 * cmd_test.c: lc test and lc submit. Works out which solution and problem
 * are meant, has the core build and judge it, and shows the progress as
 * it happens: as text, with file:line: lines an editor's quickfix list
 * understands, or as JSON Lines (docs/json.md).
 */

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cli.h"

/* Longest value shown whole in a failed case; longer ones are cut. */
#define SHOW_VALUE_MAX 1000
/* Lines of a program's stdout and stderr shown for a failed case. */
#define SHOW_LINES 40

/* Set by ^C. The core looks at it at least every 50 ms. */
static volatile sig_atomic_t cancel_flag;

static void on_sigint(int sig)
{
    (void)sig;
    cancel_flag = 1;
}

/* What is being judged. */
typedef struct target {
    const lc_problem *p;
    const lc_lang *lang;
    lc_str src;               /* the solution, an absolute path */
    lc_str src_shown;         /* the solution as the user sees it */
    lc_str dir;               /* the directory it is in */
    lc_str cases;             /* the cases file, or empty for none */
    lc_str cases_shown;
} target;

static void target_free(target *t)
{
    lc_str_free(&t->src);
    lc_str_free(&t->src_shown);
    lc_str_free(&t->dir);
    lc_str_free(&t->cases);
    lc_str_free(&t->cases_shown);
}

static int is_file(const char *path)
{
    struct stat st;

    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

static int is_dir(const char *path)
{
    struct stat st;

    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

/* realpath(3) into an lc_str: LC_OK, LC_ERR_NOMEM, or LC_ERR_IO with
 * errno set. */
static int real_path(const char *path, lc_str *out)
{
    char *r = realpath(path, NULL);
    int rc;

    if (!r)
        return errno == ENOMEM ? LC_ERR_NOMEM : LC_ERR_IO;
    lc_str_clear(out);
    rc = lc_str_puts(out, r);
    free(r);
    return rc;
}

/* Reports a failed real_path. */
static int path_error(cli *c, lc_ctx *ctx, int rc, const char *what,
                      const char *path)
{
    if (rc == LC_ERR_NOMEM)
        return cli_error(c, ctx, "out of memory");
    return cli_error(c, ctx, "cannot %s '%s': %s", what, path,
                     strerror(errno));
}

/* The problem a directory is for: its name is NNNN-slug. */
static const lc_problem *dir_problem(lc_ctx *ctx, const char *dir)
{
    const char *base = strrchr(dir, '/');

    base = base && base[1] ? base + 1 : dir;
    return lc_problem_find(ctx, base);
}

/* The solution in a directory: solution.<ext> of --lang, else the one
 * there is. */
static int pick_solution(cli *c, lc_ctx *ctx, target *t, const char *what)
{
    lc_str path = LC_STR_INIT;
    const lc_lang *found = NULL;
    size_t i;
    int rc = CLI_EXIT_OK;

    if (t->lang) {
        lc_str_printf(&t->src, "%s/solution.%s", t->dir.data, t->lang->ext);
        if (t->src.failed)
            return cli_error(c, ctx, "out of memory");
        if (!is_file(t->src.data))
            return cli_error(c, ctx, "no %s solution in %s (run 'lc start "
                             "%d --lang %s' to create one)", t->lang->title,
                             what, t->p->id, t->lang->name);
        return CLI_EXIT_OK;
    }
    for (i = 0; i < lc_lang_count() && rc == CLI_EXIT_OK; i++) {
        lc_str_clear(&path);
        lc_str_printf(&path, "%s/solution.%s", t->dir.data,
                      lc_lang_at(i)->ext);
        if (path.failed)
            rc = cli_error(c, ctx, "out of memory");
        else if (is_file(path.data) && found)
            rc = cli_error(c, ctx, "%s has both a C and a C++ solution; "
                           "choose one with --lang c or --lang cpp", what);
        else if (is_file(path.data))
            found = lc_lang_at(i);
    }
    lc_str_free(&path);
    if (rc != CLI_EXIT_OK)
        return rc;
    if (!found)
        return cli_error(c, ctx, "no solution in %s (run 'lc start %d' to "
                         "create one)", what, t->p->id);
    t->lang = found;
    lc_str_printf(&t->src, "%s/solution.%s", t->dir.data, found->ext);
    return t->src.failed ? cli_error(c, ctx, "out of memory") : CLI_EXIT_OK;
}

/*
 * Works out the problem, the language and the solution file from the
 * argument (a file, a directory, a problem) or the current directory.
 */
static int find_target(cli *c, lc_ctx *ctx, int use_cases, target *t)
{
    const char *arg = c->nargs ? c->args[0] : NULL;
    int rc;

    t->lang = cli_lang_option(c, &rc);
    if (rc != CLI_EXIT_OK)
        return rc;

    if (arg && !is_file(arg) && !is_dir(arg) &&
        (strchr(arg, '/') || lc_lang_for_path(arg)))
        return cli_error(c, ctx, "cannot find '%s'", arg);
    if (arg && is_file(arg)) {
        const char *slash;

        if (!t->lang)
            t->lang = lc_lang_for_path(arg);
        if (!t->lang)
            return cli_error(c, ctx, "can't tell the language of '%s' (use "
                             "a .c or .cpp file, or --lang)", arg);
        rc = real_path(arg, &t->src);
        if (rc != LC_OK)
            return path_error(c, ctx, rc, "find", arg);
        slash = strrchr(t->src.data, '/');
        lc_str_append(&t->dir, t->src.data,
                      slash > t->src.data ? (size_t)(slash - t->src.data) : 1);
        if (t->dir.failed)
            return cli_error(c, ctx, "out of memory");
        t->p = dir_problem(ctx, t->dir.data);
        if (!t->p)
            return cli_error(c, ctx, "can't tell which problem '%s' is for: "
                             "its directory should be named like "
                             "0001-two-sum", arg);
        lc_str_puts(&t->src_shown, arg);
    } else if (!arg || is_dir(arg)) {
        rc = real_path(arg ? arg : ".", &t->dir);
        if (rc != LC_OK)
            return path_error(c, ctx, rc, "find", arg ? arg : ".");
        t->p = dir_problem(ctx, t->dir.data);
        if (!t->p && arg)
            return cli_error(c, ctx, "'%s' is not a problem directory (one "
                             "named like 0001-two-sum)", arg);
        if (!t->p)
            return cli_error(c, ctx, "no problem given, and the current "
                             "directory is not a problem directory (one "
                             "named like 0001-two-sum)");
        rc = pick_solution(c, ctx, t, arg ? arg : "the current directory");
        if (rc != CLI_EXIT_OK)
            return rc;
        cli_show_path(&t->src_shown, t->src.data);
    } else {
        lc_str shown = LC_STR_INIT;

        t->p = lc_problem_find(ctx, arg);
        if (!t->p)
            return cli_error(c, ctx, "%s", lc_last_error(ctx));
        if (lc_problem_workspace(ctx, t->p, &t->dir) != LC_OK)
            return cli_error(c, ctx, "%s", lc_last_error(ctx));
        if (!is_dir(t->dir.data))
            return cli_error(c, ctx, "no solution for problem %d yet: %s "
                             "doesn't exist (run 'lc start %d')", t->p->id,
                             t->dir.data, t->p->id);
        cli_show_path(&shown, t->dir.data);
        rc = shown.failed ? cli_error(c, ctx, "out of memory")
                          : pick_solution(c, ctx, t, shown.data);
        lc_str_free(&shown);
        if (rc != CLI_EXIT_OK)
            return rc;
        cli_show_path(&t->src_shown, t->src.data);
    }

    if (use_cases && c->case_file) {
        rc = real_path(c->case_file, &t->cases);
        if (rc != LC_OK)
            return path_error(c, ctx, rc, "open", c->case_file);
        lc_str_puts(&t->cases_shown, c->case_file);
    } else if (use_cases) {
        lc_str_printf(&t->cases, "%s/cases.txt", t->dir.data);
        if (t->cases.failed)
            return cli_error(c, ctx, "out of memory");
        if (!is_file(t->cases.data))
            lc_str_clear(&t->cases);
        else
            cli_show_path(&t->cases_shown, t->cases.data);
    }
    if (t->src.failed || t->src_shown.failed || t->cases.failed ||
        t->cases_shown.failed)
        return cli_error(c, ctx, "out of memory");
    return CLI_EXIT_OK;
}

/* ------------------------------------------------------------------ */
/* Showing the run                                                     */
/* ------------------------------------------------------------------ */

typedef struct ui {
    cli *c;
    lc_ctx *ctx;
    target *t;
    int submit;
    int color;
    int width;                /* of the first column of the case table */
    lc_str out;               /* scratch for each piece of output */
    lc_str tmp;
    int nomem;
} ui;

static void flush(ui *u)
{
    if (u->out.failed)
        u->nomem = 1;
    else if (u->out.len)
        cli_write(u->out.data, u->out.len);
    lc_str_clear(&u->out);
}

/* Appends s with every `from` replaced by `to`. */
static void put_replaced(lc_str *out, const char *s, size_t n,
                         const char *from, const char *to)
{
    size_t fl = strlen(from), i = 0, start = 0;

    if (fl == 0 || strcmp(from, to) == 0) {
        lc_str_append(out, s, n);
        return;
    }
    while (i + fl <= n) {
        if (memcmp(s + i, from, fl) == 0) {
            lc_str_append(out, s + start, i - start);
            lc_str_puts(out, to);
            i += fl;
            start = i;
        } else {
            i++;
        }
    }
    lc_str_append(out, s + start, n - start);
}

/* Text from a program, made safe for a terminal: control characters
 * other than tabs and newlines become '?'. */
static void put_safe(lc_str *out, const char *s, size_t n)
{
    size_t i;

    for (i = 0; i < n; i++) {
        unsigned char ch = (unsigned char)s[i];
        lc_str_putc(out, (ch < 0x20 && ch != '\t' && ch != '\n') ||
                                 ch == 0x7f ? '?' : (char)ch);
    }
}

static unsigned verdict_style(lc_verdict v)
{
    switch (v) {
    case LC_ACCEPTED:
        return CLI_ST_GREEN | CLI_ST_BOLD;
    case LC_TIME_LIMIT_EXCEEDED:
    case LC_MEMORY_LIMIT_EXCEEDED:
    case LC_OUTPUT_LIMIT_EXCEEDED:
        return CLI_ST_YELLOW | CLI_ST_BOLD;
    default:
        return CLI_ST_RED | CLI_ST_BOLD;
    }
}

/* "Example 2", "Case 5" or "cases.txt:12". */
static void case_label(ui *u, const lc_case_result *r, lc_str *out)
{
    if (r->custom)
        lc_str_printf(out, "%s:%d", u->t->cases_shown.data, r->line);
    else if (!u->submit && r->example)
        lc_str_printf(out, "Example %lu", (unsigned long)r->index + 1);
    else
        lc_str_printf(out, "Case %lu", (unsigned long)r->index + 1);
}

static void build_line(ui *u, const lc_build *b)
{
    lc_str_puts(&u->out, "Building ");
    if (b->reference)
        lc_str_puts(&u->out, "the reference solution");
    else
        lc_str_puts(&u->out, u->t->src_shown.data);
    lc_str_printf(&u->out, " (%s, %s%s)\n",
                  strcmp(b->lang, "c") == 0 ? "C" : "C++", b->std,
                  b->sanitize ? ", sanitizers" : "");
}

/* The compiler's output, with the solution's path as the user knows it,
 * and a hint when the errors are in lc's harness. */
static void build_output(ui *u, const lc_build *b)
{
    size_t i;
    int harness = 0, errors = 0;

    if (b->output_len) {
        put_replaced(&u->out, b->output, b->output_len, b->source,
                     u->t->src_shown.data);
        if (b->output[b->output_len - 1] != '\n')
            lc_str_putc(&u->out, '\n');
    }
    for (i = 0; i < b->ndiags; i++) {
        if (strcmp(b->diags[i].severity, "error") != 0)
            continue;
        if (strcmp(b->diags[i].file, "lc_harness") == 0)
            harness = 1;
        else
            errors = 1;
    }
    if (harness && !b->ok && !b->reference && b->prototype)
        lc_str_printf(&u->out, "lc: errors in lc_harness, the code that "
                      "calls your solution, mean your\nfunction doesn't "
                      "match the problem's. lc calls:\n    %s\n",
                      b->prototype);
    else if (errors && !b->ok && !b->reference &&
             strcmp(b->lang, "cpp") == 0 && strcmp(b->std, "c++11") == 0)
        lc_str_puts(&u->out, "lc: this was built as C++11. If your code "
                    "needs a newer standard, use --std\nc++17 or --std "
                    "max, or set LC_CXXSTD.\n");
}

static void case_line(ui *u, const lc_case_result *r)
{
    lc_str label = LC_STR_INIT;
    const char *v = lc_verdict_name(r->verdict);

    case_label(u, r, &label);
    lc_str_printf(&u->out, "  %-*s ", u->width, lc_str_cstr(&label));
    cli_put_styled(&u->out, u->color, verdict_style(r->verdict), v);
    cli_put_spaces(&u->out, strlen(v) < 22 ? 22 - strlen(v) : 1);
    lc_str_printf(&u->out, "%5ld ms\n", r->time_ms);
    if (label.failed)
        u->nomem = 1;
    lc_str_free(&label);
}

/* A value, cut when it is very long. */
static void put_value(lc_str *out, const char *v)
{
    size_t n = strlen(v);

    if (n <= SHOW_VALUE_MAX) {
        put_safe(out, v, n);
        return;
    }
    put_safe(out, v, SHOW_VALUE_MAX);
    lc_str_printf(out, "... (%lu bytes in all)", (unsigned long)n);
}

/* Appends lines [from, to) of s, indented. */
static void put_lines(lc_str *out, const char *s, size_t from, size_t to)
{
    while (from < to) {
        const char *nl = memchr(s + from, '\n', to - from);
        size_t e = nl ? (size_t)(nl - s) : to;

        lc_str_puts(out, "    ");
        put_safe(out, s + from, e - from);
        lc_str_putc(out, '\n');
        from = nl ? e + 1 : to;
    }
}

/*
 * A program's output, indented. Past SHOW_LINES lines, the first and the
 * last SHOW_LINES / 2 are shown: a crash report has its summary at the
 * top, and the program's last words are at the bottom.
 */
static void put_text(lc_str *out, const char *label, const char *s,
                     size_t n, int truncated)
{
    size_t lines = 0, i, k, head_end, tail_start;

    if (n == 0)
        return;
    for (i = 0; i < n; i++)
        if (s[i] == '\n' || i == n - 1)
            lines++;
    lc_str_printf(out, "  %s:%s\n", label, truncated ? " (cut short)" : "");
    if (lines <= SHOW_LINES) {
        put_lines(out, s, 0, n);
        return;
    }
    for (i = 0, k = 0; i < n && k < SHOW_LINES / 2; i++)
        if (s[i] == '\n')
            k++;
    head_end = i;
    for (i = n, k = 0; i > 0; i--)
        if (s[i - 1] == '\n' && i != n && ++k == SHOW_LINES / 2)
            break;
    tail_start = i;
    put_lines(out, s, 0, head_end);
    lc_str_printf(out, "    ... (%lu lines left out)\n",
                  (unsigned long)(lines - 2 * (SHOW_LINES / 2)));
    put_lines(out, s, tail_start, n);
}

/* The details of a case that failed. */
static void failure(ui *u, const lc_report *rep, const lc_case_result *r)
{
    const lc_problem *p = u->t->p;
    size_t i;

    if (r->custom)
        lc_str_printf(&u->out, "\n%s:%d: ", u->t->cases_shown.data, r->line);
    else
        lc_str_printf(&u->out, "\n%s:1: ", u->t->src_shown.data);
    cli_put_styled(&u->out, u->color, verdict_style(r->verdict),
                   lc_verdict_name(r->verdict));
    if (r->custom)
        lc_str_puts(&u->out, " on your case");
    else if (!u->submit && r->example)
        lc_str_printf(&u->out, " on example %lu", (unsigned long)r->index + 1);
    else
        lc_str_printf(&u->out, " on case %lu of %lu",
                      (unsigned long)r->index + 1, (unsigned long)rep->total);
    if (r->message)
        lc_str_printf(&u->out, " (%s)", r->message);
    lc_str_putc(&u->out, '\n');

    lc_str_puts(&u->out, "  Input:   ");
    if (r->ninputs == p->narg_names) {
        for (i = 0; i < r->ninputs; i++) {
            lc_str_printf(&u->out, "%s%s = ", i ? ", " : " ",
                          p->arg_names[i]);
            put_value(&u->out, r->inputs[i]);
        }
        lc_str_putc(&u->out, '\n');
    } else {
        lc_str_putc(&u->out, '\n');
        for (i = 0; i < r->ninputs; i++) {
            lc_str_puts(&u->out, "    ");
            put_value(&u->out, r->inputs[i]);
            lc_str_putc(&u->out, '\n');
        }
    }
    if (r->output) {
        lc_str_puts(&u->out, "  Output:   ");
        put_value(&u->out, r->output);
        lc_str_putc(&u->out, '\n');
    }
    lc_str_puts(&u->out, "  Expected: ");
    put_value(&u->out, r->expected);
    lc_str_putc(&u->out, '\n');
    put_text(&u->out, "Stdout", r->stdout_text, r->stdout_len,
             r->stdout_truncated);
    /* Sanitizer reports name the solution: as the user knows it. */
    lc_str_clear(&u->tmp);
    put_replaced(&u->tmp, r->stderr_text, r->stderr_len, u->t->src.data,
                 u->t->src_shown.data);
    if (u->tmp.failed)
        u->nomem = 1;
    put_text(&u->out, "Stderr", lc_str_cstr(&u->tmp), u->tmp.len,
             r->stderr_truncated);
}

static void summary(ui *u, const lc_report *rep)
{
    size_t i;

    if (rep->verdict == LC_COMPILE_ERROR) {
        lc_str_putc(&u->out, '\n');
        cli_put_styled(&u->out, u->color, verdict_style(rep->verdict),
                       "Compile Error");
        lc_str_putc(&u->out, '\n');
        return;
    }
    for (i = 0; i < rep->nresults; i++)
        if (rep->results[i].verdict != LC_ACCEPTED)
            failure(u, rep, &rep->results[i]);
    lc_str_putc(&u->out, '\n');
    if (rep->cancelled) {
        lc_str_printf(&u->out, "Cancelled after %lu of %lu cases\n",
                      (unsigned long)rep->nresults,
                      (unsigned long)rep->total);
        return;
    }
    cli_put_styled(&u->out, u->color, verdict_style(rep->verdict),
                   lc_verdict_name(rep->verdict));
    lc_str_printf(&u->out, ": %lu/%lu cases passed",
                  (unsigned long)rep->passed, (unsigned long)rep->total);
    if (rep->verdict == LC_ACCEPTED)
        lc_str_printf(&u->out, ", slowest %ld ms, peak memory %.1f MB",
                      rep->max_time_ms, rep->max_memory_kb / 1024.0);
    lc_str_putc(&u->out, '\n');
}

/* ------------------------------------------------------------------ */
/* JSON Lines                                                          */
/* ------------------------------------------------------------------ */

static void json_line_begin(lc_json *j, lc_str *out, const char *event)
{
    lc_json_init(j, out);
    lc_json_begin_object(j);
    lc_json_key(j, "schema");
    lc_json_int(j, LC_JSON_SCHEMA);
    lc_json_key(j, "event");
    lc_json_string(j, event);
}

static void json_line_end(ui *u, lc_json *j)
{
    lc_json_end_object(j);
    if (lc_json_finish(j) != LC_OK || lc_str_putc(&u->out, '\n') != LC_OK)
        u->nomem = 1;
    else
        flush(u);
    lc_str_clear(&u->out);
}

void cli_json_build(lc_json *j, const lc_build *b, int done)
{
    size_t i;

    lc_json_key(j, "reference");
    lc_json_bool(j, b->reference);
    lc_json_key(j, "role");
    lc_json_string(j, b->role);
    lc_json_key(j, "lang");
    lc_json_string(j, b->lang);
    lc_json_key(j, "std");
    lc_json_string(j, b->std);
    lc_json_key(j, "source");
    lc_json_string(j, b->source);
    lc_json_key(j, "compiler");
    lc_json_string(j, b->compiler);
    lc_json_key(j, "sanitize");
    lc_json_bool(j, b->sanitize);
    if (!done)
        return;
    lc_json_key(j, "ok");
    lc_json_bool(j, b->ok);
    lc_json_key(j, "cached");
    lc_json_bool(j, b->cached);
    lc_json_key(j, "time_ms");
    lc_json_int(j, b->time_ms);
    lc_json_key(j, "output");
    lc_json_string_n(j, b->output, b->output_len);
    lc_json_key(j, "diagnostics");
    lc_json_begin_array(j);
    for (i = 0; i < b->ndiags; i++) {
        const lc_diag *d = &b->diags[i];

        lc_json_begin_object(j);
        lc_json_key(j, "file");
        lc_json_string(j, d->file);
        lc_json_key(j, "line");
        lc_json_int(j, d->line);
        lc_json_key(j, "column");
        lc_json_int(j, d->column);
        lc_json_key(j, "severity");
        lc_json_string(j, d->severity);
        lc_json_key(j, "message");
        lc_json_string(j, d->message);
        lc_json_key(j, "in_solution");
        lc_json_bool(j, d->in_solution);
        lc_json_end_object(j);
    }
    lc_json_end_array(j);
    lc_json_key(j, "prototype");
    lc_json_string(j, b->prototype);
}

static void json_case(lc_json *j, const lc_problem *p,
                      const lc_case_result *r)
{
    const char *sig = r->signal ? lc_signal_name(r->signal) : NULL;
    int named = r->ninputs == p->narg_names;
    size_t i;

    lc_json_key(j, "index");
    lc_json_int(j, (long long)r->index);
    lc_json_key(j, "file");
    lc_json_string(j, r->file);
    lc_json_key(j, "line");
    lc_json_int(j, r->line);
    lc_json_key(j, "example");
    lc_json_bool(j, r->example);
    lc_json_key(j, "custom");
    lc_json_bool(j, r->custom);
    lc_json_key(j, "verdict");
    lc_json_string(j, lc_verdict_id(r->verdict));
    lc_json_key(j, "message");
    lc_json_string(j, r->message);
    lc_json_key(j, "inputs");
    lc_json_begin_array(j);
    for (i = 0; i < r->ninputs; i++) {
        lc_json_begin_object(j);
        lc_json_key(j, "name");
        lc_json_string(j, named ? p->arg_names[i] : NULL);
        lc_json_key(j, "value");
        lc_json_string(j, r->inputs[i]);
        lc_json_end_object(j);
    }
    lc_json_end_array(j);
    lc_json_key(j, "expected");
    lc_json_string(j, r->expected);
    lc_json_key(j, "output");
    lc_json_string(j, r->output);
    lc_json_key(j, "stdout");
    lc_json_string_n(j, r->stdout_text, r->stdout_len);
    lc_json_key(j, "stdout_truncated");
    lc_json_bool(j, r->stdout_truncated);
    lc_json_key(j, "stderr");
    lc_json_string_n(j, r->stderr_text, r->stderr_len);
    lc_json_key(j, "stderr_truncated");
    lc_json_bool(j, r->stderr_truncated);
    lc_json_key(j, "time_ms");
    lc_json_int(j, r->time_ms);
    lc_json_key(j, "wall_ms");
    lc_json_int(j, r->wall_ms);
    lc_json_key(j, "memory_kb");
    lc_json_int(j, r->memory_kb);
    lc_json_key(j, "exit_code");
    if (r->signal)
        lc_json_null(j);
    else
        lc_json_int(j, r->exit_code);
    lc_json_key(j, "signal");
    if (sig)
        lc_json_string(j, sig);
    else if (r->signal)
        lc_json_int(j, r->signal);
    else
        lc_json_null(j);
}

static void on_event(void *ud, const lc_event *ev)
{
    ui *u = ud;
    lc_json j;

    if (u->c->json) {
        switch (ev->kind) {
        case LC_EV_BUILD_START:
        case LC_EV_BUILD_DONE:
            json_line_begin(&j, &u->out, ev->kind == LC_EV_BUILD_START
                                             ? "build_start"
                                             : "build_done");
            cli_json_build(&j, ev->build, ev->kind == LC_EV_BUILD_DONE);
            json_line_end(u, &j);
            break;
        case LC_EV_CASE_START:
            json_line_begin(&j, &u->out, "case_start");
            lc_json_key(&j, "index");
            lc_json_int(&j, (long long)ev->index);
            lc_json_key(&j, "total");
            lc_json_int(&j, (long long)ev->total);
            lc_json_key(&j, "file");
            lc_json_string(&j, ev->test_case->file);
            lc_json_key(&j, "line");
            lc_json_int(&j, ev->test_case->line);
            json_line_end(u, &j);
            break;
        case LC_EV_CASE_DONE:
            json_line_begin(&j, &u->out, "case_done");
            lc_json_key(&j, "total");
            lc_json_int(&j, (long long)ev->total);
            json_case(&j, u->t->p, ev->result);
            json_line_end(u, &j);
            break;
        case LC_EV_DONE:
        case LC_EV_FINDING:
            break; /* written once lc_judge returns, with the warnings */
        }
        return;
    }

    switch (ev->kind) {
    case LC_EV_BUILD_START:
        build_line(u, ev->build);
        break;
    case LC_EV_BUILD_DONE:
        build_output(u, ev->build);
        break;
    case LC_EV_CASE_DONE:
        if (!u->submit)
            case_line(u, ev->result);
        break;
    default:
        break;
    }
    flush(u);
}

/* What recording a submission did, for the done event. */
typedef struct recorded {
    int done;                 /* it was recorded */
    lc_record_info info;
    lc_str warning;           /* why it wasn't, or why there's no copy */
} recorded;

static void json_done(ui *u, const lc_report *rep, const recorded *rec)
{
    lc_json j;

    json_line_begin(&j, &u->out, "done");
    lc_json_key(&j, "verdict");
    lc_json_string(&j, lc_verdict_id(rep->verdict));
    lc_json_key(&j, "passed");
    lc_json_int(&j, (long long)rep->passed);
    lc_json_key(&j, "total");
    lc_json_int(&j, (long long)rep->total);
    lc_json_key(&j, "cancelled");
    lc_json_bool(&j, rep->cancelled);
    lc_json_key(&j, "max_time_ms");
    lc_json_int(&j, rep->max_time_ms);
    lc_json_key(&j, "max_memory_kb");
    lc_json_int(&j, rep->max_memory_kb);
    if (u->submit) {
        lc_json_key(&j, "status");
        lc_json_string(&j, rec->done ? lc_status_name(rec->info.after.status)
                                     : NULL);
        lc_json_key(&j, "first_solve");
        lc_json_bool(&j, rec->done && rec->info.after.status == LC_SOLVED &&
                             rec->info.before.status != LC_SOLVED);
        lc_json_key(&j, "saved");
        lc_json_string(&j, rec->info.saved.len ? rec->info.saved.data : NULL);
    }
    cli_json_warnings(&j, u->ctx, rec->warning.len ? rec->warning.data
                                                   : NULL);
    json_line_end(u, &j);
}

/* Records a submission in your progress and history (lc submit). */
static void record(cli *c, lc_ctx *ctx, const target *t,
                   const lc_report *rep, recorded *rec)
{
    int rc;

    if (rep->cancelled || rep->verdict == LC_VERDICT_NONE)
        return;
    rc = lc_record(ctx, t->p, t->lang, t->src.data, rep, c->sanitize,
                   &rec->info);
    rec->done = rec->info.recorded;
    if (rc == LC_OK)
        return;
    if (rec->done)
        lc_str_printf(&rec->warning, "the submission was recorded, but not "
                      "saved in the history: %s", lc_last_error(ctx));
    else
        lc_str_printf(&rec->warning, "the submission was not recorded in "
                      "your progress: %s", lc_last_error(ctx));
    if (!rec->warning.failed)
        cli_warn(rec->warning.data);
}

/* ------------------------------------------------------------------ */
/* The commands                                                        */
/* ------------------------------------------------------------------ */

static int exit_status(const lc_report *rep)
{
    if (rep->cancelled)
        return CLI_EXIT_CANCELLED;
    switch (rep->verdict) {
    case LC_ACCEPTED:
        return CLI_EXIT_OK;
    case LC_COMPILE_ERROR:
        return CLI_EXIT_COMPILE;
    default:
        return CLI_EXIT_VERDICT;
    }
}

/* Width of the case table's first column: "Example 10", or the longest
 * "cases.txt:LINE". */
static int label_width(const target *t, const lc_cases *mine)
{
    size_t w = 10, i;

    for (i = 0; mine && i < mine->count; i++) {
        char num[24];
        size_t n = t->cases_shown.len + 1 +
                   (size_t)snprintf(num, sizeof num, "%d",
                                    mine->items[i].line);
        if (n > w)
            w = n;
    }
    return w > 60 ? 60 : (int)w;
}

/* Reports the core's last error. In text, paths the user gave appear as
 * given. */
static int judge_error(cli *c, lc_ctx *ctx, const target *t)
{
    lc_str a = LC_STR_INIT, b = LC_STR_INIT;
    const char *msg = lc_last_error(ctx);
    int rc;

    if (c->json)
        return cli_error(c, ctx, "%s", msg);
    put_replaced(&a, msg, strlen(msg), t->src.data, t->src_shown.data);
    if (t->cases.len)
        put_replaced(&b, lc_str_cstr(&a), a.len, t->cases.data,
                     t->cases_shown.data);
    else
        lc_str_append(&b, lc_str_cstr(&a), a.len);
    rc = b.failed ? cli_error(c, ctx, "out of memory")
                  : cli_error(c, ctx, "%s", b.data);
    lc_str_free(&a);
    lc_str_free(&b);
    return rc;
}

static int run(cli *c, int submit)
{
    lc_ctx *ctx;
    target t;
    lc_cases *mine = NULL;
    lc_judge_options opts;
    lc_hooks hooks;
    lc_report *rep = NULL;
    struct sigaction sa, old;
    recorded rec;
    ui u;
    int rc;

    memset(&t, 0, sizeof t);
    memset(&u, 0, sizeof u);
    memset(&rec, 0, sizeof rec);
    rc = cli_load(c, &ctx);
    if (rc != CLI_EXIT_OK)
        return rc;
    rc = find_target(c, ctx, !submit, &t);
    if (rc != CLI_EXIT_OK)
        goto done;
    if (t.cases.len && lc_load_user_cases(ctx, t.cases.data, &mine) != LC_OK) {
        rc = judge_error(c, ctx, &t);
        goto done;
    }

    memset(&opts, 0, sizeof opts);
    opts.std = c->std;
    opts.sanitize = c->sanitize;
    opts.examples = !submit;
    opts.all = submit;
    opts.cases = mine;
    opts.stop_on_failure = submit;
    u.c = c;
    u.ctx = ctx;
    u.t = &t;
    u.submit = submit;
    u.color = !c->json && cli_color_enabled(c->color);
    memset(&hooks, 0, sizeof hooks);
    hooks.on_event = on_event;
    hooks.ud = &u;
    hooks.cancel = &cancel_flag;

    cancel_flag = 0;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_sigint;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, &old);
    u.width = label_width(&t, mine);
    rc = lc_judge(ctx, t.p, t.lang, t.src.data, &opts, &hooks, &rep);
    sigaction(SIGINT, &old, NULL);
    if (rc != LC_OK) {
        rc = judge_error(c, ctx, &t);
        goto done;
    }
    if (c->json) {
        if (submit)
            record(c, ctx, &t, rep, &rec);
        json_done(&u, rep, &rec);
    } else {
        summary(&u, rep);
        flush(&u);
        if (submit)
            record(c, ctx, &t, rep, &rec);
    }
    rc = u.nomem ? cli_error(c, ctx, "out of memory") : exit_status(rep);
done:
    lc_record_info_free(&rec.info);
    lc_str_free(&rec.warning);
    lc_report_free(rep);
    lc_cases_free(mine);
    lc_str_free(&u.out);
    lc_str_free(&u.tmp);
    target_free(&t);
    lc_close(ctx);
    return rc;
}

int cmd_test(cli *c)
{
    return run(c, 0);
}

int cmd_submit(cli *c)
{
    return run(c, 1);
}
