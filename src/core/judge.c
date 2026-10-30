/*
 * judge.c: judging a built program against test cases. It sets the limits
 * for each case, turns what happened to the program into a verdict,
 * reports progress events and builds the report (docs/judging.md).
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

/* Stack for a case: deep recursion is common in tree problems. */
#define CASE_STACK (256ull * LC_MIB)
/* Files a case may write. Solutions have no reason to write any. */
#define CASE_FSIZE (16ull * LC_MIB)

/* ------------------------------------------------------------------ */
/* Names                                                               */
/* ------------------------------------------------------------------ */

static const char *const verdict_names[] = {
    "No Verdict", "Accepted", "Wrong Answer", "Compile Error",
    "Runtime Error", "Time Limit Exceeded", "Memory Limit Exceeded",
    "Output Limit Exceeded"
};

static const char *const verdict_ids[] = {
    "none", "accepted", "wrong_answer", "compile_error", "runtime_error",
    "time_limit_exceeded", "memory_limit_exceeded", "output_limit_exceeded"
};

const char *lc_verdict_name(lc_verdict v)
{
    if ((unsigned)v >= sizeof verdict_names / sizeof verdict_names[0])
        v = LC_VERDICT_NONE;
    return verdict_names[v];
}

const char *lc_verdict_id(lc_verdict v)
{
    if ((unsigned)v >= sizeof verdict_ids / sizeof verdict_ids[0])
        v = LC_VERDICT_NONE;
    return verdict_ids[v];
}

/* The standard ones, plus the common extensions where a system has them. */
static const struct {
    int sig;
    const char *name;
} signal_names[] = {
    {SIGABRT, "SIGABRT"}, {SIGALRM, "SIGALRM"}, {SIGBUS, "SIGBUS"},
    {SIGCHLD, "SIGCHLD"}, {SIGCONT, "SIGCONT"}, {SIGFPE, "SIGFPE"},
    {SIGHUP, "SIGHUP"},   {SIGILL, "SIGILL"},   {SIGINT, "SIGINT"},
    {SIGKILL, "SIGKILL"}, {SIGPIPE, "SIGPIPE"}, {SIGQUIT, "SIGQUIT"},
    {SIGSEGV, "SIGSEGV"}, {SIGSTOP, "SIGSTOP"}, {SIGTERM, "SIGTERM"},
    {SIGTSTP, "SIGTSTP"}, {SIGTTIN, "SIGTTIN"}, {SIGTTOU, "SIGTTOU"},
    {SIGUSR1, "SIGUSR1"}, {SIGUSR2, "SIGUSR2"},
#ifdef SIGSYS
    {SIGSYS, "SIGSYS"},
#endif
#ifdef SIGTRAP
    {SIGTRAP, "SIGTRAP"},
#endif
#ifdef SIGXCPU
    {SIGXCPU, "SIGXCPU"},
#endif
#ifdef SIGXFSZ
    {SIGXFSZ, "SIGXFSZ"},
#endif
#ifdef SIGVTALRM
    {SIGVTALRM, "SIGVTALRM"},
#endif
#ifdef SIGPROF
    {SIGPROF, "SIGPROF"},
#endif
#ifdef SIGURG
    {SIGURG, "SIGURG"},
#endif
};

const char *lc_signal_name(int sig)
{
    size_t i;

    for (i = 0; i < sizeof signal_names / sizeof signal_names[0]; i++)
        if (signal_names[i].sig == sig)
            return signal_names[i].name;
    return NULL;
}

/* ------------------------------------------------------------------ */
/* Limits and verdicts                                                 */
/* ------------------------------------------------------------------ */

void lc_limits_init(lc_limits *l, int time_ms, int memory_mb)
{
    l->time_ms = time_ms;
    l->memory_mb = memory_mb;
    l->output_max = LC_RUN_OUTPUT_MAX;
    l->result_max = LC_RESULT_MAX;
}

void lc_limits_apply(const lc_limits *l, lc_run_spec *spec)
{
    spec->cpu_sec = 0;
    spec->wall_ms = 0;
    if (l->time_ms > 0) {
        /* RLIMIT_CPU counts whole seconds. It only stops a program that
         * spins; the verdict compares the CPU time with time_ms. */
        spec->cpu_sec = (int)((l->time_ms + 999LL) / 1000 + 1);
        /* For a program that sleeps or blocks, with room for a busy
         * machine. */
        spec->wall_ms = 2LL * l->time_ms + 1000;
    }
    /*
     * Not the memory limit itself: Memory Limit Exceeded is decided from
     * the peak resident memory. The address-space limit only stops a
     * runaway program soon after it passes the memory limit, with enough
     * room above it that a vector doubling past the limit gets there
     * instead of failing to allocate.
     */
    spec->as_bytes = l->memory_mb > 0
                         ? (2ull * (unsigned)l->memory_mb + 64) * LC_MIB
                         : 0;
    spec->stack_bytes = CASE_STACK;
    spec->fsize_bytes = CASE_FSIZE;
    spec->stdout_max = l->output_max;
    spec->stderr_max = l->output_max;
    spec->result_max = l->result_max;
}

/* "1 MiB", "16 MiB", or "1000 bytes". */
static void put_size(lc_str *s, size_t n)
{
    if (n >= LC_MIB && n % LC_MIB == 0)
        lc_str_printf(s, "%lu MiB", (unsigned long)(n / LC_MIB));
    else
        lc_str_printf(s, "%lu bytes", (unsigned long)n);
}

lc_verdict lc_classify(const lc_limits *l, const lc_run_result *r,
                       lc_str *why)
{
    lc_str_clear(why);
    if (r->overflow) {
        if (r->overflow & LC_RUN_STDOUT) {
            lc_str_puts(why, "printed more than ");
            put_size(why, l->output_max);
            lc_str_puts(why, " on stdout");
        } else if (r->overflow & LC_RUN_STDERR) {
            lc_str_puts(why, "printed more than ");
            put_size(why, l->output_max);
            lc_str_puts(why, " on stderr");
        } else {
            lc_str_puts(why, "returned a result longer than ");
            put_size(why, l->result_max);
        }
        return LC_OUTPUT_LIMIT_EXCEEDED;
    }
    if (l->memory_mb > 0 &&
        (r->mem_killed || r->max_rss_kb > (long)l->memory_mb * 1024)) {
        long mb = (r->max_rss_kb + 1023) / 1024;

        if (mb > l->memory_mb)
            lc_str_printf(why, "used %ld MB of memory; the limit is %d MB",
                          mb, l->memory_mb);
        else
            lc_str_printf(why, "used over %d MB of memory; the limit is "
                          "%d MB", 2 * l->memory_mb + 64, l->memory_mb);
        return LC_MEMORY_LIMIT_EXCEEDED;
    }
    if (l->time_ms > 0 && (r->cpu_ms > l->time_ms
#ifdef SIGXCPU
                           || r->signal == SIGXCPU
#endif
                           )) {
        lc_str_printf(why, "used %ld ms of CPU time; the limit is %d ms",
                      r->cpu_ms, l->time_ms);
        return LC_TIME_LIMIT_EXCEEDED;
    }
    if (r->timed_out) {
        lc_str_printf(why, "was still running after %ld ms (it used %ld ms "
                      "of CPU time; the limit is %d ms)", r->wall_ms,
                      r->cpu_ms, l->time_ms);
        return LC_TIME_LIMIT_EXCEEDED;
    }
    if (r->signal) {
        const char *name = lc_signal_name(r->signal);

        if (name)
            lc_str_printf(why, "killed by %s", name);
        else
            lc_str_printf(why, "killed by signal %d", r->signal);
        return LC_RUNTIME_ERROR;
    }
    if (!r->exited || r->exit_code != 0) {
        lc_str_printf(why, "exited with status %d", r->exit_code);
        return LC_RUNTIME_ERROR;
    }
    return LC_VERDICT_NONE;
}

/* ------------------------------------------------------------------ */
/* Reports                                                             */
/* ------------------------------------------------------------------ */

typedef struct report_priv {
    lc_arena arena;
} report_priv;

void lc_report_free(lc_report *report)
{
    if (!report)
        return;
    if (report->priv_) {
        report_priv *rp = report->priv_;
        lc_arena_free(&rp->arena);
        free(rp);
    }
    free(report);
}

/* A NUL-terminated copy of n bytes that may include NULs. */
static char *copy_bytes(lc_arena *a, const char *p, size_t n)
{
    char *s = lc_arena_alloc(a, n + 1);

    if (!s)
        return NULL;
    if (n)
        memcpy(s, p, n);
    s[n] = '\0';
    return s;
}

/* Keeps the first LC_REPORT_TEXT_MAX bytes of a program's stdout. */
static int keep_head(lc_arena *a, const lc_str *s, int cut,
                     const char **text, size_t *len, int *truncated)
{
    size_t n = s->len < LC_REPORT_TEXT_MAX ? s->len : LC_REPORT_TEXT_MAX;

    *text = copy_bytes(a, lc_str_cstr(s), n);
    *len = n;
    *truncated = cut || n < s->len;
    return *text ? LC_OK : LC_ERR_NOMEM;
}

/* Keeps the last LC_REPORT_TEXT_MAX bytes of stderr, from the start of a
 * line when there is one in reach. */
static int keep_tail(lc_arena *a, const lc_str *s, int cut,
                     const char **text, size_t *len, int *truncated)
{
    const char *p = lc_str_cstr(s);
    size_t from = 0;

    if (s->len > LC_REPORT_TEXT_MAX) {
        const char *nl;

        from = s->len - LC_REPORT_TEXT_MAX;
        if (p[from - 1] != '\n') {
            nl = memchr(p + from, '\n', s->len - from);
            if (nl && (size_t)(nl - p) + 1 < s->len)
                from = (size_t)(nl - p) + 1;
        }
    }
    *text = copy_bytes(a, p + from, s->len - from);
    *len = s->len - from;
    *truncated = cut || from > 0;
    return *text ? LC_OK : LC_ERR_NOMEM;
}

/* ------------------------------------------------------------------ */
/* Judging                                                             */
/* ------------------------------------------------------------------ */

static void emit(const lc_hooks *hooks, const lc_event *ev)
{
    if (hooks && hooks->on_event)
        hooks->on_event(hooks->ud, ev);
}

static int cancel_set(const lc_hooks *hooks)
{
    return hooks && hooks->cancel && *hooks->cancel;
}

static int is_space(int c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* The type of input i: a parameter of the signature, then a hidden one. */
static const lc_type *input_type(const lc_problem *p, size_t i)
{
    if (i < p->sig->nparams)
        return &p->sig->params[i].type;
    return &p->hidden[i - p->sig->nparams].type;
}

/* Per-run state shared by the cases. */
typedef struct judge_run {
    lc_ctx *ctx;
    const lc_problem *p;
    lc_limits lim;
    lc_run_spec spec;
    lc_run_result run;
    lc_str input;
    lc_str why;
    lc_str text;
    lc_arena *arena;          /* the report's */
} judge_run;

/*
 * Looks at the result of a run that exited cleanly: decodes it, then
 * compares it with the expected one. Sets the verdict, and `output` when
 * the result could be read. Returns LC_ERR_NOMEM or LC_OK.
 */
static int check_result(judge_run *jr, const lc_value *expected,
                        lc_pool *pool, lc_case_result *cr)
{
    const lc_str *res = &jr->run.result;
    const char *text = lc_str_cstr(res);
    const lc_value *got;
    size_t pos = 0, i;
    int rc, match = 0;

    for (i = 0; i < res->len && is_space((unsigned char)text[i]); i++)
        ;
    if (i == res->len) {
        cr->verdict = LC_RUNTIME_ERROR;
        lc_str_puts(&jr->why, "exited without returning a result");
        return jr->why.failed ? LC_ERR_NOMEM : LC_OK;
    }
    rc = lc_wire_decode(jr->ctx, pool, &jr->p->sig->ret, text, res->len,
                        &pos, &got);
    if (rc == LC_ERR_NOMEM)
        return rc;
    if (rc != LC_OK) {
        cr->verdict = LC_RUNTIME_ERROR;
        lc_str_printf(&jr->why, "its result could not be read: %s",
                      lc_last_error(jr->ctx));
        return jr->why.failed ? LC_ERR_NOMEM : LC_OK;
    }
    while (pos < res->len && is_space((unsigned char)text[pos]))
        pos++;
    if (pos < res->len) {
        cr->verdict = LC_RUNTIME_ERROR;
        lc_str_printf(&jr->why, "its result has unexpected text at byte %lu",
                      (unsigned long)pos + 1);
        return jr->why.failed ? LC_ERR_NOMEM : LC_OK;
    }

    lc_str_clear(&jr->text);
    if (lc_value_print(got, &jr->text) != LC_OK)
        return LC_ERR_NOMEM;
    cr->output = copy_bytes(jr->arena, jr->text.data, jr->text.len);
    if (!cr->output)
        return LC_ERR_NOMEM;
    rc = lc_value_match(jr->ctx, &jr->p->compare, expected, got, &match);
    if (rc != LC_OK)
        return rc;
    cr->verdict = match ? LC_ACCEPTED : LC_WRONG_ANSWER;
    return LC_OK;
}

/*
 * Runs one case and fills in *cr. Returns LC_OK, also when the run was
 * cancelled (jr->run.cancelled), or an error.
 */
static int judge_case(judge_run *jr, const lc_case *c, size_t index,
                      lc_case_result *cr)
{
    lc_ctx *ctx = jr->ctx;
    const lc_value *args, *expected;
    lc_pool *pool;
    char **inputs;
    size_t i;
    int rc;

    pool = lc_pool_new();
    if (!pool)
        return lc_oom(ctx);
    rc = lc_case_values(ctx, jr->p, c, pool, &args, &expected);
    if (rc != LC_OK)
        goto done;

    lc_str_clear(&jr->input);
    for (i = 0; i < c->nargs; i++) {
        rc = lc_wire_encode(ctx, input_type(jr->p, i), &args[i], &jr->input);
        if (rc != LC_OK)
            goto done;
        lc_str_putc(&jr->input, '\n');
    }
    if (jr->input.failed) {
        rc = lc_oom(ctx);
        goto done;
    }
    jr->spec.input = jr->input.data;
    jr->spec.input_len = jr->input.len;

    rc = lc_run(ctx, &jr->spec, &jr->run);
    if (rc != LC_OK || jr->run.cancelled)
        goto done;

    memset(cr, 0, sizeof *cr);
    cr->index = index;
    cr->line = c->line;
    cr->example = c->example;
    cr->time_ms = jr->run.cpu_ms;
    cr->wall_ms = jr->run.wall_ms;
    cr->memory_kb = jr->run.max_rss_kb;
    cr->exit_code = jr->run.exited ? jr->run.exit_code : -1;
    cr->signal = jr->run.signal;

    cr->verdict = lc_classify(&jr->lim, &jr->run, &jr->why);
    if (cr->verdict == LC_VERDICT_NONE) {
        rc = check_result(jr, expected, pool, cr);
        if (rc != LC_OK)
            goto nomem;
    }
    if (jr->why.failed)
        goto nomem;
    if (jr->why.len) {
        cr->message = copy_bytes(jr->arena, jr->why.data, jr->why.len);
        if (!cr->message)
            goto nomem;
    }

    inputs = lc_arena_array(jr->arena, c->nargs ? c->nargs : 1,
                            sizeof *inputs);
    if (!inputs)
        goto nomem;
    for (i = 0; i < c->nargs; i++) {
        inputs[i] = lc_arena_strdup(jr->arena, c->args[i]);
        if (!inputs[i])
            goto nomem;
    }
    cr->inputs = (const char *const *)inputs;
    cr->ninputs = c->nargs;
    lc_str_clear(&jr->text);
    if (lc_value_print(expected, &jr->text) != LC_OK)
        goto nomem;
    cr->expected = copy_bytes(jr->arena, jr->text.data, jr->text.len);
    if (!cr->expected ||
        keep_head(jr->arena, &jr->run.out, (jr->run.overflow & LC_RUN_STDOUT) != 0,
                  &cr->stdout_text, &cr->stdout_len,
                  &cr->stdout_truncated) != LC_OK ||
        keep_tail(jr->arena, &jr->run.err, (jr->run.overflow & LC_RUN_STDERR) != 0,
                  &cr->stderr_text, &cr->stderr_len,
                  &cr->stderr_truncated) != LC_OK)
        goto nomem;
    rc = LC_OK;
    goto done;

nomem:
    rc = lc_oom(ctx);
done:
    lc_pool_free(pool);
    return rc;
}

int lc_judge_exe(lc_ctx *ctx, const lc_problem *p, const char *exe,
                 const lc_case *cases, size_t ncases,
                 const lc_judge_opts *opts, const lc_hooks *hooks,
                 lc_report **out)
{
    judge_run jr;
    lc_report *report;
    report_priv *rp;
    lc_case_result *results;
    lc_event ev;
    size_t i;
    int rc = LC_OK, time_ms, memory_mb;

    if (!out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_judge_exe: bad arguments");
    *out = NULL;
    if (!p || !exe || (ncases && !cases))
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_judge_exe: bad arguments");
    if (!p->sig)
        return lc_set_error(ctx, LC_ERR_INVALID, "problem %d (%s) has no "
                            "signature, so it can't be judged", p->id,
                            p->slug);
    if (p->sig->ret.kind == LC_T_VOID)
        return lc_set_error(ctx, LC_ERR_INVALID, "problem %d (%s): judging a "
                            "function that returns void is not supported "
                            "yet", p->id, p->slug);
    if (p->compare.mode == LC_CMP_CHECKER)
        return lc_set_error(ctx, LC_ERR_INVALID, "problem %d (%s): compare: "
                            "checker is not supported yet", p->id, p->slug);
    if (ncases == 0)
        return lc_set_error(ctx, LC_ERR_INVALID, "problem %d (%s) has no "
                            "test cases to run", p->id, p->slug);

    report = calloc(1, sizeof *report);
    rp = calloc(1, sizeof *rp);
    if (!report || !rp) {
        free(report);
        free(rp);
        return lc_oom(ctx);
    }
    report->priv_ = rp;
    results = lc_arena_array(&rp->arena, ncases, sizeof *results);
    if (!results) {
        lc_report_free(report);
        return lc_oom(ctx);
    }
    report->results = results;
    report->total = ncases;

    memset(&jr, 0, sizeof jr);
    jr.ctx = ctx;
    jr.p = p;
    jr.arena = &rp->arena;
    time_ms = opts && opts->time_ms > 0 ? opts->time_ms : p->time_limit_ms;
    memory_mb = opts && opts->memory_mb > 0 ? opts->memory_mb
                                            : p->memory_limit_mb;
    lc_limits_init(&jr.lim, time_ms, memory_mb);
    jr.spec.path = exe;
    jr.spec.cwd = opts ? opts->cwd : NULL;
    jr.spec.result_fd = 1;
    jr.spec.cancel = hooks ? hooks->cancel : NULL;
    lc_limits_apply(&jr.lim, &jr.spec);
    lc_run_result_init(&jr.run);

    for (i = 0; i < ncases; i++) {
        lc_case_result *cr = &results[report->nresults];

        if (cancel_set(hooks)) {
            report->cancelled = 1;
            break;
        }
        memset(&ev, 0, sizeof ev);
        ev.kind = LC_EV_CASE_START;
        ev.index = i;
        ev.total = ncases;
        ev.test_case = &cases[i];
        emit(hooks, &ev);

        rc = judge_case(&jr, &cases[i], i, cr);
        if (rc != LC_OK)
            break;
        if (jr.run.cancelled) {
            report->cancelled = 1;
            break;
        }
        report->nresults++;
        if (cr->verdict == LC_ACCEPTED)
            report->passed++;
        else if (report->verdict == LC_VERDICT_NONE)
            report->verdict = cr->verdict;
        if (cr->time_ms > report->max_time_ms)
            report->max_time_ms = cr->time_ms;
        if (cr->memory_kb > report->max_memory_kb)
            report->max_memory_kb = cr->memory_kb;

        memset(&ev, 0, sizeof ev);
        ev.kind = LC_EV_CASE_DONE;
        ev.index = i;
        ev.total = ncases;
        ev.result = cr;
        emit(hooks, &ev);
        if (opts && opts->stop_on_failure && cr->verdict != LC_ACCEPTED)
            break;
    }

    lc_run_result_free(&jr.run);
    lc_str_free(&jr.input);
    lc_str_free(&jr.why);
    lc_str_free(&jr.text);
    if (rc != LC_OK) {
        lc_report_free(report);
        return rc;
    }
    if (report->verdict == LC_VERDICT_NONE && !report->cancelled)
        report->verdict = LC_ACCEPTED;

    memset(&ev, 0, sizeof ev);
    ev.kind = LC_EV_DONE;
    ev.report = report;
    emit(hooks, &ev);
    *out = report;
    return LC_OK;
}
