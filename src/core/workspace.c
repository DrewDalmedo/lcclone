#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * workspace.c: the directories where you write solutions
 * (docs/workspace.md):
 *
 *   ~/lc/                     the workspace ($LC_WORKSPACE)
 *     .clangd                 written by lc, for clangd
 *     .lcclone/               the preludes .clangd points at
 *     0001-two-sum/
 *       README.md             the statement; rewritten by lc
 *       solution.cpp          yours: written once, from the stub
 *       cases.txt             yours: written once, with instructions
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "internal.h"

/* The first line of a .clangd that lc wrote. */
#define CLANGD_MARK "# Written by lc"
/* Where the copies of the preludes go, under the workspace. */
#define PRELUDE_DIR ".lcclone"

const char *lc_workspace_dir(lc_ctx *ctx)
{
    const char *v;

    if (ctx->workspace_dir)
        return ctx->workspace_dir;
    if (ctx->opt_workspace_dir) {
        ctx->workspace_dir = ctx->opt_workspace_dir;
    } else if ((v = lc_getenv("LC_WORKSPACE")) != NULL) {
        ctx->workspace_dir = lc_arena_strdup(&ctx->arena, v);
    } else if ((v = lc_getenv("HOME")) != NULL) {
        ctx->workspace_dir = lc_path_join(&ctx->arena, v, "lc");
    } else {
        lc_set_error(ctx, LC_ERR_NOTFOUND, "no workspace directory: set "
                     "LC_WORKSPACE or HOME");
        return NULL;
    }
    if (!ctx->workspace_dir)
        lc_oom(ctx);
    return ctx->workspace_dir;
}

int lc_problem_workspace(lc_ctx *ctx, const lc_problem *p, lc_str *out)
{
    const char *ws;

    if (!p || !out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_problem_workspace: bad "
                            "arguments");
    ws = lc_workspace_dir(ctx);
    if (!ws)
        return ctx->err_nomem ? LC_ERR_NOMEM : LC_ERR_NOTFOUND;
    lc_str_printf(out, "%s%s%04d-%s", ws,
                  ws[0] && ws[strlen(ws) - 1] == '/' ? "" : "/", p->id,
                  p->slug);
    return out->failed ? lc_oom(ctx) : LC_OK;
}

int lc_solution_path(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
                     lc_str *out)
{
    int rc;

    if (!lang)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_solution_path: bad "
                            "arguments");
    rc = lc_problem_workspace(ctx, p, out);
    if (rc != LC_OK)
        return rc;
    lc_str_printf(out, "/solution.%s", lang->ext);
    return out->failed ? lc_oom(ctx) : LC_OK;
}

void lc_start_info_free(lc_start_info *info)
{
    if (!info)
        return;
    lc_str_free(&info->dir);
    lc_str_free(&info->solution);
    lc_str_free(&info->readme);
    lc_str_free(&info->cases);
}

/* ------------------------------------------------------------------ */
/* README.md                                                           */
/* ------------------------------------------------------------------ */

static const char *difficulty_title(lc_difficulty d)
{
    switch (d) {
    case LC_EASY:
        return "Easy";
    case LC_MEDIUM:
        return "Medium";
    case LC_HARD:
        return "Hard";
    default:
        return "Unknown";
    }
}

static void readme_examples(lc_str *s, const lc_problem *p,
                            const lc_cases *cases)
{
    size_t i, k, n = 0;

    for (i = 0; cases && i < cases->count; i++) {
        const lc_case *c = &cases->items[i];

        if (!c->example)
            continue;
        if (n)
            lc_str_putc(s, '\n');
        lc_str_printf(s, "**Example %lu:**\n\n```\nInput:",
                      (unsigned long)++n);
        if (c->nargs == p->narg_names) {
            for (k = 0; k < c->nargs; k++)
                lc_str_printf(s, "%s%s = %s", k ? ", " : " ",
                              p->arg_names[k], c->args[k]);
            lc_str_putc(s, '\n');
        } else {
            lc_str_putc(s, '\n');
            for (k = 0; k < c->nargs; k++)
                lc_str_printf(s, "%s\n", c->args[k]);
        }
        lc_str_printf(s, "Output: %s\n```\n", c->expected ? c->expected
                                                         : "");
        if (c->explanation)
            lc_str_printf(s, "\n**Explanation:** %s\n", c->explanation);
    }
}

int lc_readme(lc_ctx *ctx, const lc_problem *p, const lc_cases *cases,
              lc_str *out)
{
    lc_md_doc *doc = NULL;
    const char *st, *at = NULL;
    size_t i, n;
    int line = 0;

    if (!p || !out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_readme: bad arguments");
    lc_str_printf(out, "# %d. %s\n\n**Difficulty:** %s", p->id, p->title,
                  difficulty_title(p->difficulty));
    if (p->ntags) {
        lc_str_puts(out, " \xc2\xb7 **Tags:** ");
        for (i = 0; i < p->ntags; i++)
            lc_str_printf(out, "%s%s", i ? ", " : "", p->tags[i]);
    }
    lc_str_puts(out, "\n\n");

    /* The statement, with the examples where its @examples line is. */
    if (lc_md_parse(p->statement, p->statement_len, &doc) != LC_OK)
        return lc_oom(ctx);
    for (i = 0; i < doc->nblocks && !line; i++)
        if (doc->blocks[i].kind == LC_MD_EXAMPLES)
            line = doc->blocks[i].line;
    lc_md_free(doc);
    st = p->statement;
    n = p->statement_len;
    if (line > 0) {
        int l = 1;

        for (at = st; l < line && at < st + n; at++)
            if (*at == '\n')
                l++;
    }
    if (at) {
        const char *nl = memchr(at, '\n', (size_t)(st + n - at));

        lc_str_append(out, st, (size_t)(at - st));
        readme_examples(out, p, cases);
        if (nl)
            lc_str_append(out, nl + 1, (size_t)(st + n - nl - 1));
    } else {
        lc_str_append(out, st, n);
        if (n && st[n - 1] != '\n')
            lc_str_putc(out, '\n');
        if (cases) {
            lc_str_putc(out, '\n');
            readme_examples(out, p, cases);
        }
    }
    if (out->len && out->data[out->len - 1] != '\n')
        lc_str_putc(out, '\n');
    lc_str_puts(out, "\n---\n\nIn this directory, `lc test` runs your solution "
                "on the examples and on your\nown cases in `cases.txt`, and "
                "`lc submit` runs it on every case.\n");
    return out->failed ? lc_oom(ctx) : LC_OK;
}

/* ------------------------------------------------------------------ */
/* Files                                                               */
/* ------------------------------------------------------------------ */

/* Writes a file unless it already holds exactly this text. */
static int write_if_changed(lc_ctx *ctx, const char *path, const char *data,
                            size_t len)
{
    lc_str old = LC_STR_INIT;
    int rc = lc_read_file(ctx, path, len + 1, &old), same;

    same = rc == LC_OK && old.len == len && memcmp(old.data, data, len) == 0;
    lc_str_free(&old);
    if (rc == LC_ERR_NOMEM)
        return rc;
    if (same)
        return LC_OK;
    return lc_write_file_atomic(ctx, path, data, len);
}

/* Creates a file that must not exist yet; *created is 0 if it did. */
static int create_new(lc_ctx *ctx, const char *path, const char *data,
                      size_t len, int *created)
{
    int fd, e;
    size_t done = 0;

    *created = 0;
    do
        fd = open(path, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0666);
    while (fd < 0 && errno == EINTR);
    if (fd < 0) {
        if (errno == EEXIST)
            return LC_OK;
        return lc_set_errno(ctx, LC_ERR_IO, errno, "cannot create '%s'",
                            path);
    }
    while (done < len) {
        ssize_t n = write(fd, data + done, len - done);

        if (n < 0) {
            if (errno == EINTR)
                continue;
            e = errno;
            close(fd);
            unlink(path);
            return lc_set_errno(ctx, LC_ERR_IO, e, "cannot write '%s'", path);
        }
        done += (size_t)n;
    }
    if (close(fd) != 0) {
        e = errno;
        unlink(path);
        return lc_set_errno(ctx, LC_ERR_IO, e, "cannot write '%s'", path);
    }
    *created = 1;
    return LC_OK;
}

/* The text of a new cases.txt. */
static void cases_template(lc_str *s, const lc_problem *p,
                           const lc_cases *cases)
{
    char tname[LC_TYPE_NAME_MAX];
    const lc_case *ex = NULL;
    size_t i, w = 0;

    lc_str_printf(s, "# Your own cases for %d. %s. 'lc test' runs them "
                  "after the examples.\n#\n# A case is one line per input, "
                  "in this order:\n#\n", p->id, p->title);
    for (i = 0; i < p->sig->nparams; i++)
        if (strlen(p->sig->params[i].name) > w)
            w = strlen(p->sig->params[i].name);
    for (i = 0; i < p->sig->nparams; i++)
        lc_str_printf(s, "#   %-*s  %s\n", (int)w, p->sig->params[i].name,
                      lc_type_name(p->sig->params[i].type, tname));
    lc_str_puts(s,
                "#\n"
                "# then, if you like, a line '=> expected result'. Without "
                "it, the expected\n"
                "# result comes from lc's reference solution. Leave a blank "
                "line between\n"
                "# cases. Values are written as in the examples: 7, "
                "[1,2,3], \"text\",\n"
                "# true, 2.5. Lines starting with '#' are comments.\n");
    for (i = 0; cases && i < cases->count && !ex; i++)
        if (cases->items[i].example &&
            cases->items[i].nargs == p->sig->nparams)
            ex = &cases->items[i];
    if (ex) {
        lc_str_puts(s, "#\n# For example:\n#\n");
        for (i = 0; i < ex->nargs; i++)
            lc_str_printf(s, "# %s\n", ex->args[i]);
        lc_str_printf(s, "# => %s\n", ex->expected);
    }
}

/* "path" for YAML: double-quoted, with '"' and '\' escaped. */
static void yaml_quote(lc_str *s, const char *v)
{
    lc_str_putc(s, '"');
    for (; *v; v++) {
        if (*v == '"' || *v == '\\')
            lc_str_putc(s, '\\');
        lc_str_putc(s, *v);
    }
    lc_str_putc(s, '"');
}

/* The standard .clangd should tell clangd about: what a build would use
 * (lc_std_resolve with no --std), or the default when that can't be
 * worked out, with a warning. */
static int clangd_std(lc_ctx *ctx, const lc_lang *lang, const char **std)
{
    int rc = lc_std_resolve(ctx, lang, NULL, std);

    if (rc == LC_ERR_NOMEM)
        return rc;
    if (rc != LC_OK) {
        *std = lang->default_std;
        if (lc_getenv(lang->std_env))
            return lc_add_warning(ctx, ".clangd uses %s: %s", *std,
                                  lc_last_error(ctx));
    }
    return LC_OK;
}

/* .clangd and the prelude copies at the top of the workspace. */
static int write_clangd(lc_ctx *ctx, const char *ws)
{
    static const struct {
        lc_lang_id lang;
        const char *rt;
        const char *file;
        const char *match;
        const char *x;
        const char *extra;    /* another runtime file it needs, or NULL */
    } parts[] = {
        {LC_LANG_C, "c/lc_prelude.h", "lc_prelude.h", ".*\\.c", "-xc",
         "c/uthash.h"},
        {LC_LANG_CPP, "cpp/lc_prelude.hpp", "lc_prelude.hpp",
         ".*\\.(cpp|cc|cxx)", "-xc++", NULL},
    };
    lc_str path = LC_STR_INIT, text = LC_STR_INIT, old = LC_STR_INIT;
    size_t i;
    int rc;

    lc_str_printf(&text,
                  "%s. It tells clangd about lc's preludes and the "
                  "standards lc\n"
                  "# builds with, for the solution files here. lc rewrites "
                  "it as needed; to keep\n"
                  "# changes of your own, delete this first line and lc "
                  "will leave it alone.\n", CLANGD_MARK);
    for (i = 0; i < sizeof parts / sizeof parts[0]; i++) {
        const lc_rt_file *f = lc_rt_file_get(parts[i].rt);
        const char *std;

        lc_str_clear(&path);
        lc_str_printf(&path, "%s/%s", ws, PRELUDE_DIR);
        if (path.failed || !f) {
            rc = f ? lc_oom(ctx) : lc_set_error(ctx, LC_ERR_INVALID,
                                                "internal error: no %s",
                                                parts[i].rt);
            goto done;
        }
        rc = lc_mkdir_p(ctx, path.data, 0777);
        if (rc != LC_OK)
            goto done;
        if (parts[i].extra) {
            const lc_rt_file *x = lc_rt_file_get(parts[i].extra);
            const char *base = strrchr(parts[i].extra, '/') + 1;
            size_t keep = path.len;

            lc_str_printf(&path, "/%s", base);
            if (path.failed || !x) {
                rc = x ? lc_oom(ctx)
                       : lc_set_error(ctx, LC_ERR_INVALID, "internal error: "
                                      "no %s", parts[i].extra);
                goto done;
            }
            rc = write_if_changed(ctx, path.data, x->text, x->len);
            if (rc != LC_OK)
                goto done;
            path.len = keep;
            path.data[keep] = '\0';
        }
        lc_str_printf(&path, "/%s", parts[i].file);
        if (path.failed) {
            rc = lc_oom(ctx);
            goto done;
        }
        rc = write_if_changed(ctx, path.data, f->text, f->len);
        if (rc == LC_OK)
            rc = clangd_std(ctx, lc_lang_at(parts[i].lang), &std);
        if (rc != LC_OK)
            goto done;
        lc_str_printf(&text, "%sIf:\n  PathMatch: %s\nCompileFlags:\n"
                      "  Add: [%s, -std=%s, ", i ? "---\n" : "",
                      parts[i].match, parts[i].x, std);
        if (parts[i].extra) {
            /* For a solution that includes uthash.h itself: the
             * directory part of `path`. */
            size_t dlen = path.len - strlen(parts[i].file) - 1;

            lc_str_puts(&text, "-I, ");
            path.data[dlen] = '\0';
            yaml_quote(&text, path.data);
            path.data[dlen] = '/';
            lc_str_puts(&text, ", ");
        }
        lc_str_puts(&text, "-include, ");
        yaml_quote(&text, path.data);
        lc_str_puts(&text, "]\n");
    }
    lc_str_clear(&path);
    lc_str_printf(&path, "%s/.clangd", ws);
    if (path.failed || text.failed) {
        rc = lc_oom(ctx);
        goto done;
    }
    rc = lc_read_file(ctx, path.data, LC_MIB, &old);
    if (rc == LC_ERR_NOMEM)
        goto done;
    if (rc == LC_OK && strncmp(lc_str_cstr(&old), CLANGD_MARK,
                               sizeof CLANGD_MARK - 1) != 0) {
        rc = lc_add_warning(ctx, "left %s alone: lc didn't write it, so "
                            "clangd may not know about lc's preludes",
                            path.data);
        goto done;
    }
    rc = write_if_changed(ctx, path.data, text.data, text.len);
done:
    lc_str_free(&path);
    lc_str_free(&text);
    lc_str_free(&old);
    return rc;
}

int lc_start(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
             lc_start_info *out)
{
    lc_cases *cases = NULL;
    lc_str text = LC_STR_INIT;
    const char *ws;
    int rc;

    if (!p || !lang || !out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_start: bad arguments");
    memset(out, 0, sizeof *out);
    if (!p->sig)
        return lc_set_error(ctx, LC_ERR_INVALID, "problem %d (%s) has no "
                            "signature, so there is nothing to start from",
                            p->id, p->slug);
    ws = lc_workspace_dir(ctx);
    if (!ws)
        return ctx->err_nomem ? LC_ERR_NOMEM : LC_ERR_NOTFOUND;

    /* Without its tests.txt, the README just has no examples. */
    rc = lc_load_cases(ctx, p, &cases);
    if (rc == LC_ERR_NOMEM)
        goto fail;
    if (rc != LC_OK) {
        cases = NULL;
        rc = lc_add_warning(ctx, "%s", lc_last_error(ctx));
        if (rc != LC_OK)
            goto fail;
    }

    rc = lc_problem_workspace(ctx, p, &out->dir);
    if (rc == LC_OK)
        rc = lc_mkdir_p(ctx, out->dir.data, 0777);
    if (rc != LC_OK)
        goto fail;
    lc_str_printf(&out->readme, "%s/README.md", out->dir.data);
    lc_str_printf(&out->cases, "%s/cases.txt", out->dir.data);
    if (out->readme.failed || out->cases.failed) {
        rc = lc_oom(ctx);
        goto fail;
    }
    rc = lc_solution_path(ctx, p, lang, &out->solution);
    if (rc == LC_OK)
        rc = lc_readme(ctx, p, cases, &text);
    if (rc == LC_OK)
        rc = write_if_changed(ctx, out->readme.data, text.data, text.len);
    if (rc != LC_OK)
        goto fail;

    lc_str_clear(&text);
    rc = lc_stub(ctx, p, lang, &text);
    if (rc == LC_OK)
        rc = create_new(ctx, out->solution.data, text.data, text.len,
                        &out->created_solution);
    if (rc != LC_OK)
        goto fail;
    lc_str_clear(&text);
    cases_template(&text, p, cases);
    if (text.failed) {
        rc = lc_oom(ctx);
        goto fail;
    }
    rc = create_new(ctx, out->cases.data, text.data, text.len,
                    &out->created_cases);
    if (rc == LC_OK)
        rc = write_clangd(ctx, ws);
    if (rc != LC_OK)
        goto fail;
    lc_str_free(&text);
    lc_cases_free(cases);
    return LC_OK;
fail:
    lc_str_free(&text);
    lc_cases_free(cases);
    lc_start_info_free(out);
    memset(out, 0, sizeof *out);
    return rc;
}
