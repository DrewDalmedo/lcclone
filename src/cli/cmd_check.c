/* realpath() is only declared for the XSI level on glibc. */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

/*
 * cmd_check.c: lc check, for problem authors. Checks packs with the
 * core's lc_check and shows what it finds as file:line: messages (which
 * editors understand) or as JSON Lines (docs/json.md).
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cli.h"

static volatile sig_atomic_t cancel_flag;

static void on_sigint(int sig)
{
    (void)sig;
    cancel_flag = 1;
}

/* One pack to check: a loaded problem, or the directory of a pack that
 * failed to load (to find the loader's warnings about it). */
typedef struct pack {
    const lc_problem *p;      /* NULL for a pack that failed to load */
    const char *dir;          /* absolute; malloc'ed when p is NULL */
} pack;

typedef struct run {
    cli *c;
    lc_ctx *ctx;
    int color;
    const char *pack;         /* the pack being checked, for JSON */
    const char *dir;          /* its directory, for findings without a file */
    lc_str out;
    int nomem;
} run;

static void flush(run *r)
{
    if (r->out.failed)
        r->nomem = 1;
    else if (r->out.len)
        cli_write(r->out.data, r->out.len);
    lc_str_clear(&r->out);
}

static unsigned level_style(lc_finding_level l)
{
    return l == LC_FINDING_ERROR     ? CLI_ST_RED | CLI_ST_BOLD
           : l == LC_FINDING_WARNING ? CLI_ST_YELLOW | CLI_ST_BOLD
                                     : CLI_ST_BOLD;
}

static void json_begin(lc_json *j, lc_str *out, const char *event)
{
    lc_json_init(j, out);
    lc_json_begin_object(j);
    lc_json_key(j, "schema");
    lc_json_int(j, LC_JSON_SCHEMA);
    lc_json_key(j, "event");
    lc_json_string(j, event);
}

static void json_end(run *r, lc_json *j)
{
    lc_json_end_object(j);
    if (lc_json_finish(j) != LC_OK || lc_str_putc(&r->out, '\n') != LC_OK)
        r->nomem = 1;
    else
        flush(r);
    lc_str_clear(&r->out);
}

/* Shows a finding: "file:line: level: message", then the detail. */
static void show_finding(run *r, const lc_finding *f, const char *dir)
{
    const char *d, *nl;
    lc_json j;

    if (r->c->json) {
        json_begin(&j, &r->out, "finding");
        lc_json_key(&j, "pack");
        lc_json_string(&j, r->pack);
        lc_json_key(&j, "level");
        lc_json_string(&j, lc_finding_level_name(f->level));
        lc_json_key(&j, "file");
        lc_json_string(&j, f->file);
        lc_json_key(&j, "line");
        if (f->line)
            lc_json_int(&j, f->line);
        else
            lc_json_null(&j);
        lc_json_key(&j, "message");
        lc_json_string(&j, f->message);
        lc_json_key(&j, "detail");
        lc_json_string(&j, f->detail);
        json_end(r, &j);
        return;
    }
    cli_show_path(&r->out, f->file ? f->file : dir ? dir : ".");
    if (f->file && f->line)
        lc_str_printf(&r->out, ":%d", f->line);
    lc_str_puts(&r->out, ": ");
    cli_put_styled(&r->out, r->color, level_style(f->level),
                   lc_finding_level_name(f->level));
    lc_str_printf(&r->out, ": %s\n", f->message);
    for (d = f->detail; d && *d; d = nl ? nl + 1 : NULL) {
        nl = strchr(d, '\n');
        lc_str_puts(&r->out, "    ");
        lc_str_append(&r->out, d, nl ? (size_t)(nl - d) : strlen(d));
        lc_str_putc(&r->out, '\n');
    }
    flush(r);
}

static void on_event(void *ud, const lc_event *ev)
{
    run *r = ud;
    lc_json j;

    if (ev->kind == LC_EV_FINDING) {
        show_finding(r, ev->finding, r->dir);
        return;
    }
    if (!r->c->json ||
        (ev->kind != LC_EV_BUILD_START && ev->kind != LC_EV_BUILD_DONE))
        return;
    json_begin(&j, &r->out, ev->kind == LC_EV_BUILD_START ? "build_start"
                                                          : "build_done");
    lc_json_key(&j, "pack");
    lc_json_string(&j, r->pack);
    cli_json_build(&j, ev->build, ev->kind == LC_EV_BUILD_DONE);
    json_end(r, &j);
}

/* ------------------------------------------------------------------ */
/* Which packs                                                         */
/* ------------------------------------------------------------------ */

static int is_dir(const char *path)
{
    struct stat st;

    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

/* 1 if the argument names a directory rather than an ID or slug. */
static int is_path(const char *arg)
{
    return strchr(arg, '/') != NULL || is_dir(arg);
}

/*
 * The problem directory the arguments need: the parent of the pack
 * directories they name, all the same one; NULL (and LC_OK) when they
 * name none. *out is malloc'ed.
 */
static int arg_root(cli *c, char **out)
{
    size_t i;

    *out = NULL;
    for (i = 0; i < c->nargs; i++) {
        char *real, *slash;

        if (!is_path(c->args[i]))
            continue;
        real = realpath(c->args[i], NULL);
        if (!real) {
            int e = errno;
            free(*out);
            *out = NULL;
            return cli_error(c, NULL, "cannot check '%s': %s", c->args[i],
                             strerror(e));
        }
        if (!is_dir(real)) {
            free(real);
            free(*out);
            *out = NULL;
            return cli_error(c, NULL, "cannot check '%s': not a pack "
                             "directory", c->args[i]);
        }
        slash = strrchr(real, '/');
        if (slash == real)
            slash[1] = '\0';
        else
            *slash = '\0';
        if (*out && strcmp(*out, real) != 0) {
            free(real);
            free(*out);
            *out = NULL;
            return cli_error(c, NULL, "the packs to check must be in one "
                             "problem directory");
        }
        if (*out)
            free(real);
        else
            *out = real;
    }
    return CLI_EXIT_OK;
}

/* The pack directory named NNNN-slug in `root`, or NULL. */
static const lc_problem *by_dir_name(lc_ctx *ctx, const char *name)
{
    size_t i;

    for (i = 0; i < lc_problem_count(ctx); i++) {
        const lc_problem *p = lc_problem_at(ctx, i);
        const char *base = strrchr(p->dir, '/');

        if (base && strcmp(base + 1, name) == 0)
            return p;
    }
    return NULL;
}

/* 1 if warning `w` is about the pack in directory `dir`. */
static int warning_about(const char *w, const char *dir)
{
    size_t n = strlen(dir);

    return strncmp(w, dir, n) == 0 && (w[n] == '/' || w[n] == ':');
}

/* The packs to check, in a malloc'ed array. */
static int select_packs(cli *c, lc_ctx *ctx, pack **out, size_t *n)
{
    const char *dir = lc_problems_dir(ctx);
    size_t i, k, nw = lc_warning_count(ctx), cap = 0;
    pack *packs = NULL;
    int rc = CLI_EXIT_OK;

    *out = NULL;
    *n = 0;
    if (c->nargs == 0) {
        cap = lc_problem_count(ctx) + nw + 1;
        packs = calloc(cap, sizeof *packs);
        if (!packs)
            return cli_error(c, ctx, "out of memory");
        for (i = 0; i < lc_problem_count(ctx); i++) {
            packs[*n].p = lc_problem_at(ctx, i);
            packs[*n].dir = packs[*n].p->dir;
            (*n)++;
        }
        /* The packs that failed to load: one entry per directory. */
        for (i = 0; i < nw; i++) {
            const char *w = lc_warning_at(ctx, i), *e;
            size_t dl = strlen(dir);
            char *d;

            if (strncmp(w, dir, dl) != 0 || w[dl] != '/' ||
                w[dl + 1] < '0' || w[dl + 1] > '9')
                continue;
            e = strchr(w + dl + 1, '/');
            if (!e)
                e = strchr(w + dl + 1, ':');
            if (!e)
                continue;
            for (k = 0; k < *n; k++)
                if (strlen(packs[k].dir) == (size_t)(e - w) &&
                    strncmp(packs[k].dir, w, (size_t)(e - w)) == 0)
                    break;
            if (k < *n)
                continue;
            d = malloc((size_t)(e - w) + 1);
            if (!d) {
                rc = cli_error(c, ctx, "out of memory");
                goto fail;
            }
            memcpy(d, w, (size_t)(e - w));
            d[e - w] = '\0';
            packs[*n].dir = d;
            (*n)++;
        }
        *out = packs;
        return CLI_EXIT_OK;
    }

    packs = calloc(c->nargs, sizeof *packs);
    if (!packs)
        return cli_error(c, ctx, "out of memory");
    for (i = 0; i < c->nargs; i++) {
        const char *arg = c->args[i];
        const lc_problem *p;

        if (is_path(arg)) {
            char *real = realpath(arg, NULL), *base;

            if (!real) {
                rc = cli_error(c, ctx, "out of memory");
                goto fail;
            }
            base = strrchr(real, '/');
            p = by_dir_name(ctx, base ? base + 1 : real);
            packs[*n].p = p;
            packs[*n].dir = real;
            if (p) {
                packs[*n].dir = p->dir;
                free(real);
            } else {
                /* Not loaded: it must at least be a pack that failed. */
                for (k = 0; k < nw; k++)
                    if (warning_about(lc_warning_at(ctx, k), real))
                        break;
                if (k == nw) {
                    rc = cli_error(c, ctx, "'%s' is not a problem pack (a "
                                   "directory named like 0001-two-sum, with "
                                   "a problem.md)", arg);
                    free(real);
                    goto fail;
                }
            }
        } else {
            p = lc_problem_find(ctx, arg);
            if (!p) {
                rc = cli_error(c, ctx, "%s", lc_last_error(ctx));
                goto fail;
            }
            packs[*n].p = p;
            packs[*n].dir = p->dir;
        }
        (*n)++;
    }
    *out = packs;
    return CLI_EXIT_OK;
fail:
    for (k = 0; k < *n; k++)
        if (!packs[k].p)
            free((char *)packs[k].dir);
    free(packs);
    *n = 0;
    return rc;
}

/* ------------------------------------------------------------------ */
/* Checking                                                            */
/* ------------------------------------------------------------------ */

typedef struct totals {
    size_t packs, ok, skipped, failed;
} totals;

/* The name a pack goes by: its directory's. */
static const char *pack_name(const pack *pk)
{
    const char *base = strrchr(pk->dir, '/');

    return base ? base + 1 : pk->dir;
}

static void pack_done_json(run *r, const pack *pk, const char *status,
                           const lc_check_report *rep)
{
    lc_json j;

    json_begin(&j, &r->out, "pack_done");
    lc_json_key(&j, "pack");
    lc_json_string(&j, pack_name(pk));
    lc_json_key(&j, "status");
    lc_json_string(&j, status);
    lc_json_key(&j, "errors");
    lc_json_int(&j, rep ? (long long)rep->errors : 1);
    lc_json_key(&j, "warnings");
    lc_json_int(&j, rep ? (long long)rep->warnings : 0);
    if (rep) {
        lc_json_key(&j, "skipped");
        lc_json_string(&j, rep->skipped);
        lc_json_key(&j, "cases");
        lc_json_int(&j, (long long)rep->ncases);
        lc_json_key(&j, "examples");
        lc_json_int(&j, (long long)rep->nexamples);
        lc_json_key(&j, "generated");
        lc_json_int(&j, (long long)rep->ngenerated);
        lc_json_key(&j, "ref");
        lc_json_string(&j, rep->ref);
        lc_json_key(&j, "brute");
        lc_json_string(&j, rep->brute);
        lc_json_key(&j, "gen");
        lc_json_string(&j, rep->gen);
        lc_json_key(&j, "stubs");
        lc_json_int(&j, (long long)rep->stubs);
        lc_json_key(&j, "ref_passed");
        lc_json_int(&j, (long long)rep->ref_passed);
        lc_json_key(&j, "ref_max_ms");
        lc_json_int(&j, rep->ref_max_ms);
        lc_json_key(&j, "cross");
        lc_json_int(&j, (long long)rep->cross);
        lc_json_key(&j, "regen");
        if (r->c->regen) {
            lc_json_begin_object(&j);
            lc_json_key(&j, "written");
            lc_json_bool(&j, rep->regen_written);
            lc_json_key(&j, "changed");
            lc_json_int(&j, (long long)rep->regen_changed);
            lc_json_key(&j, "generated");
            lc_json_int(&j, (long long)rep->regen_generated);
            lc_json_end_object(&j);
        } else {
            lc_json_null(&j);
        }
    }
    json_end(r, &j);
}

/* The summary line of a pack that was checked. */
static void pack_done_text(run *r, const pack *pk, const lc_check_report *rep)
{
    lc_str_printf(&r->out, "%s: ", pack_name(pk));
    if (!rep->ok) {
        cli_put_styled(&r->out, r->color, CLI_ST_RED | CLI_ST_BOLD, "FAILED");
        lc_str_printf(&r->out, " (%lu error%s)\n", (unsigned long)rep->errors,
                      rep->errors == 1 ? "" : "s");
        flush(r);
        return;
    }
    cli_put_styled(&r->out, r->color, CLI_ST_GREEN | CLI_ST_BOLD, "ok");
    lc_str_printf(&r->out, " (%lu case%s", (unsigned long)rep->ncases,
                  rep->ncases == 1 ? "" : "s");
    if (rep->skipped)
        lc_str_puts(&r->out, "; not judged yet");
    else if (rep->ref)
        lc_str_printf(&r->out, "; %s slowest %ld ms", rep->ref,
                      rep->ref_max_ms);
    if (rep->brute && rep->cross)
        lc_str_printf(&r->out, "; agrees with %s on %lu random cases",
                      rep->brute, (unsigned long)rep->cross);
    lc_str_puts(&r->out, ")\n");
    flush(r);
}

/* Shows an error message written "<file>:<line>: <message>" or
 * "<file>: <message>" (from the loader) as an error finding. */
static void show_error_text(run *r, const char *w, const char *dir)
{
    const char *colon = w[0] == '/' ? strstr(w, ": ") : NULL;
    lc_str file = LC_STR_INIT;
    lc_finding f;

    memset(&f, 0, sizeof f);
    f.level = LC_FINDING_ERROR;
    f.message = w;
    if (colon) {
        const char *num = colon;

        while (num > w && num[-1] >= '0' && num[-1] <= '9')
            num--;
        if (num > w + 1 && num[-1] == ':' && num < colon) {
            lc_str_append(&file, w, (size_t)(num - 1 - w));
            f.line = atoi(num);
        } else {
            lc_str_append(&file, w, (size_t)(colon - w));
        }
        f.message = colon + 2;
    }
    f.file = file.len ? file.data : NULL;
    if (file.failed)
        r->nomem = 1;
    else
        show_finding(r, &f, dir);
    lc_str_free(&file);
}

/* A pack that failed to load: the loader's warnings about it, as errors. */
static void unloaded(run *r, const pack *pk, totals *t)
{
    size_t i, n = lc_warning_count(r->ctx);

    r->pack = pack_name(pk);
    for (i = 0; i < n; i++)
        if (warning_about(lc_warning_at(r->ctx, i), pk->dir))
            show_error_text(r, lc_warning_at(r->ctx, i), pk->dir);
    t->failed++;
    if (r->c->json) {
        pack_done_json(r, pk, "failed", NULL);
    } else {
        lc_str_printf(&r->out, "%s: ", pack_name(pk));
        cli_put_styled(&r->out, r->color, CLI_ST_RED | CLI_ST_BOLD, "FAILED");
        lc_str_puts(&r->out, " (it does not load)\n");
        flush(r);
    }
}

int cmd_check(cli *c)
{
    lc_options opt;
    lc_ctx *ctx = NULL;
    lc_manifest *m = NULL;
    lc_check_options co;
    lc_hooks hooks;
    pack *packs = NULL;
    char *root = NULL;
    struct sigaction sa, old;
    totals t;
    run r;
    lc_json j;
    size_t n = 0, i;
    int rc, manifest_bad = 0;

    memset(&r, 0, sizeof r);
    memset(&t, 0, sizeof t);
    rc = arg_root(c, &root);
    if (rc != CLI_EXIT_OK)
        return rc;
    lc_options_init(&opt);
    opt.problems_dir = root;
    /* Packs that fail to load are reported as errors, not warnings. */
    c->quiet_packs = 1;
    rc = cli_load_with(c, &opt, &ctx);
    free(root);
    if (rc != CLI_EXIT_OK)
        return rc;
    r.c = c;
    r.ctx = ctx;
    r.color = !c->json && cli_color_enabled(c->color);
    rc = select_packs(c, ctx, &packs, &n);
    if (rc != CLI_EXIT_OK)
        goto done;

    /* A broken manifest fails the check, but the packs are still
     * checked, without it. */
    rc = lc_load_manifest(ctx, &m);
    if (rc == LC_ERR_NOMEM) {
        rc = cli_error(c, ctx, "out of memory");
        goto done;
    }
    if (rc != LC_OK) {
        manifest_bad = 1;
        r.pack = NULL;
        show_error_text(&r, lc_last_error(ctx), lc_problems_dir(ctx));
        m = NULL;
    }

    memset(&co, 0, sizeof co);
    co.regen = c->regen;
    co.count = c->count;
    co.seed = c->seed_given ? c->seed : 1;
    memset(&hooks, 0, sizeof hooks);
    hooks.on_event = on_event;
    hooks.ud = &r;
    hooks.cancel = &cancel_flag;
    cancel_flag = 0;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_sigint;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, &old);

    for (i = 0; i < n && !cancel_flag && !r.nomem; i++) {
        lc_check_report *rep = NULL;

        t.packs++;
        if (!packs[i].p) {
            unloaded(&r, &packs[i], &t);
            continue;
        }
        r.pack = pack_name(&packs[i]);
        r.dir = packs[i].dir;
        if (c->json) {
            json_begin(&j, &r.out, "pack_start");
            lc_json_key(&j, "pack");
            lc_json_string(&j, r.pack);
            lc_json_key(&j, "id");
            lc_json_int(&j, packs[i].p->id);
            lc_json_key(&j, "dir");
            lc_json_string(&j, packs[i].dir);
            json_end(&r, &j);
        }
        if (lc_check(ctx, packs[i].p, m, &co, &hooks, &rep) != LC_OK) {
            sigaction(SIGINT, &old, NULL);
            rc = cli_error(c, ctx, "%s", lc_last_error(ctx));
            goto done;
        }
        if (rep->cancelled) {
            lc_check_report_free(rep);
            t.packs--;
            break;
        }
        if (!rep->ok)
            t.failed++;
        else if (rep->skipped)
            t.skipped++;
        else
            t.ok++;
        if (c->json)
            pack_done_json(&r, &packs[i], !rep->ok       ? "failed"
                                          : rep->skipped ? "skipped"
                                                         : "ok",
                           rep);
        else
            pack_done_text(&r, &packs[i], rep);
        lc_check_report_free(rep);
    }
    sigaction(SIGINT, &old, NULL);

    if (c->json) {
        json_begin(&j, &r.out, "done");
        lc_json_key(&j, "packs");
        lc_json_int(&j, (long long)t.packs);
        lc_json_key(&j, "ok");
        lc_json_int(&j, (long long)t.ok);
        lc_json_key(&j, "skipped");
        lc_json_int(&j, (long long)t.skipped);
        lc_json_key(&j, "failed");
        lc_json_int(&j, (long long)t.failed);
        lc_json_key(&j, "manifest_ok");
        lc_json_bool(&j, !manifest_bad);
        lc_json_key(&j, "cancelled");
        lc_json_bool(&j, cancel_flag != 0);
        /* The warnings, except those about packs: those were findings. */
        lc_json_key(&j, "warnings");
        lc_json_begin_array(&j);
        for (i = 0; i < lc_warning_count(ctx); i++) {
            const char *w = lc_warning_at(ctx, i);
            const char *dir = lc_problems_dir(ctx);

            if (!(dir && strncmp(w, dir, strlen(dir)) == 0 &&
                  w[strlen(dir)] == '/'))
                lc_json_string(&j, w);
        }
        lc_json_end_array(&j);
        json_end(&r, &j);
    } else {
        lc_str_putc(&r.out, '\n');
        if (cancel_flag)
            lc_str_printf(&r.out, "Cancelled after %lu of %lu packs\n",
                          (unsigned long)t.packs, (unsigned long)n);
        else
            lc_str_printf(&r.out, "Checked %lu pack%s: %lu ok, %lu not "
                          "judged yet, %lu failed%s\n",
                          (unsigned long)t.packs, t.packs == 1 ? "" : "s",
                          (unsigned long)t.ok, (unsigned long)t.skipped,
                          (unsigned long)t.failed,
                          manifest_bad ? "; MANIFEST.tsv is broken" : "");
        flush(&r);
    }
    if (r.nomem)
        rc = cli_error(c, ctx, "out of memory");
    else if (cancel_flag)
        rc = CLI_EXIT_CANCELLED;
    else
        rc = t.failed || manifest_bad ? CLI_EXIT_VERDICT : CLI_EXIT_OK;
done:
    for (i = 0; packs && i < n; i++)
        if (!packs[i].p)
            free((char *)packs[i].dir);
    free(packs);
    lc_manifest_free(m);
    lc_str_free(&r.out);
    lc_close(ctx);
    return rc;
}
