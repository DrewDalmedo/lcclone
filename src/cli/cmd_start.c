#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/*
 * cmd_start.c: lc start. Sets up the problem's directory in the workspace
 * (the core's lc_start) and opens the solution in your editor.
 */

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

/* The language of the one solution file already in the problem's
 * directory, or NULL when there is none or there are several. */
static const lc_lang *existing_lang(lc_ctx *ctx, const lc_problem *p)
{
    const lc_lang *found = NULL;
    lc_str path = LC_STR_INIT;
    size_t i;

    for (i = 0; i < lc_lang_count(); i++) {
        struct stat st;

        lc_str_clear(&path);
        if (lc_solution_path(ctx, p, lc_lang_at(i), &path) != LC_OK)
            break;
        if (stat(path.data, &st) == 0 && S_ISREG(st.st_mode)) {
            if (found) {
                found = NULL;
                break;
            }
            found = lc_lang_at(i);
        }
    }
    lc_str_free(&path);
    return found;
}

/*
 * Runs $VISUAL, else $EDITOR, else vi, through /bin/sh (so the value may
 * have arguments), on `file` in `dir`, and waits for it. Like system(),
 * lc ignores ^C meanwhile: it is the editor's.
 */
static int run_editor(cli *c, lc_ctx *ctx, const char *dir, const char *file)
{
    const char *ed = getenv("VISUAL");
    struct sigaction ign, old_int, old_quit;
    lc_str script = LC_STR_INIT;
    pid_t pid;
    int st = 0, rc = CLI_EXIT_OK;

    if (!ed || !*ed)
        ed = getenv("EDITOR");
    if (!ed || !*ed)
        ed = "vi";
    if (lc_str_printf(&script, "%s \"$@\"", ed) != LC_OK)
        return cli_error(c, ctx, "out of memory");

    memset(&ign, 0, sizeof ign);
    ign.sa_handler = SIG_IGN;
    sigemptyset(&ign.sa_mask);
    sigaction(SIGINT, &ign, &old_int);
    sigaction(SIGQUIT, &ign, &old_quit);
    pid = fork();
    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGPIPE, SIG_DFL);
        if (chdir(dir) != 0)
            _exit(126);
        execl("/bin/sh", "sh", "-c", script.data, "sh", file, (char *)NULL);
        _exit(127);
    }
    if (pid < 0) {
        rc = cli_error(c, ctx, "cannot start the editor: %s",
                       strerror(errno));
    } else {
        while (waitpid(pid, &st, 0) < 0 && errno == EINTR)
            ;
        if (WIFEXITED(st) && WEXITSTATUS(st) == 127)
            rc = cli_error(c, ctx, "cannot run the editor '%s' (set VISUAL "
                           "or EDITOR, or use --no-edit)", ed);
        else if (WIFEXITED(st) && WEXITSTATUS(st) == 126)
            rc = cli_error(c, ctx, "cannot run the editor '%s' in '%s'", ed,
                           dir);
        else if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
            fprintf(stderr, "lc: warning: the editor '%s' exited with "
                    "status %d\n", ed,
                    WIFEXITED(st) ? WEXITSTATUS(st) : 128 + WTERMSIG(st));
    }
    sigaction(SIGINT, &old_int, NULL);
    sigaction(SIGQUIT, &old_quit, NULL);
    lc_str_free(&script);
    return rc;
}

static void start_json(lc_json *j, lc_ctx *ctx, const lc_problem *p,
                       const lc_lang *lang, const lc_start_info *info)
{
    lc_json_begin_object(j);
    lc_json_key(j, "schema");
    lc_json_int(j, LC_JSON_SCHEMA);
    lc_json_key(j, "problem");
    lc_json_begin_object(j);
    lc_json_key(j, "id");
    lc_json_int(j, p->id);
    lc_json_key(j, "slug");
    lc_json_string(j, p->slug);
    lc_json_key(j, "title");
    lc_json_string(j, p->title);
    lc_json_end_object(j);
    lc_json_key(j, "lang");
    lc_json_string(j, lang->name);
    lc_json_key(j, "dir");
    lc_json_string(j, info->dir.data);
    lc_json_key(j, "solution");
    lc_json_string(j, info->solution.data);
    lc_json_key(j, "readme");
    lc_json_string(j, info->readme.data);
    lc_json_key(j, "cases");
    lc_json_string(j, info->cases.data);
    lc_json_key(j, "created_solution");
    lc_json_bool(j, info->created_solution);
    lc_json_key(j, "created_cases");
    lc_json_bool(j, info->created_cases);
    cli_json_warnings(j, ctx, NULL);
    lc_json_end_object(j);
}

int cmd_start(cli *c)
{
    lc_ctx *ctx;
    const lc_problem *p;
    const lc_lang *lang;
    lc_start_info info;
    lc_str out = LC_STR_INIT;
    lc_json j;
    size_t nwarn, i;
    int rc;

    lang = cli_lang_option(c, &rc);
    if (rc != CLI_EXIT_OK)
        return rc;
    rc = cli_load(c, &ctx);
    if (rc != CLI_EXIT_OK)
        return rc;
    memset(&info, 0, sizeof info);
    p = lc_problem_find(ctx, c->args[0]);
    if (!p) {
        rc = cli_error(c, ctx, "%s", lc_last_error(ctx));
        goto done;
    }
    if (!lang)
        lang = existing_lang(ctx, p);
    if (!lang)
        lang = lc_lang_find("cpp");

    nwarn = lc_warning_count(ctx);
    if (lc_start(ctx, p, lang, &info) != LC_OK) {
        rc = cli_error(c, ctx, "%s", lc_last_error(ctx));
        goto done;
    }
    for (i = nwarn; i < lc_warning_count(ctx); i++)
        cli_warn(lc_warning_at(ctx, i));
    if (!lc_lang_installed(ctx, lang))
        fprintf(stderr, "lc: warning: no %s compiler was found ($%s, else "
                "%s); 'lc test' needs one\n", lang->title,
                lang->compiler_env, lang->compiler);

    if (c->json) {
        lc_json_init(&j, &out);
        start_json(&j, ctx, p, lang, &info);
        rc = cli_emit_json(c, ctx, &j, &out);
    } else if (c->no_edit) {
        lc_str_printf(&out, "%s\n", info.solution.data);
        rc = out.failed ? cli_error(c, ctx, "out of memory")
                        : cli_write(out.data, out.len);
    } else {
        const char *base = strrchr(info.solution.data, '/');

        rc = run_editor(c, ctx, info.dir.data,
                        base ? base + 1 : info.solution.data);
    }
done:
    lc_start_info_free(&info);
    lc_str_free(&out);
    lc_close(ctx);
    return rc;
}
