#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/*
 * cmd_calibrate.c: lc calibrate. Measures how fast this machine runs lc's
 * fixed loop, which sets how much time limits are scaled here
 * (docs/judging.md, "Slower machines"), or with --show says what is in use.
 */

#include <stdio.h>
#include <string.h>

#include "cli.h"

/* Where a setting comes from, for people. */
static const char *source_text(lc_scale_source s)
{
    switch (s) {
    case LC_SCALE_OPTION:
        return "from --time-scale";
    case LC_SCALE_ENV:
        return "from $LC_TIME_SCALE";
    case LC_SCALE_CONFIG:
        return "from the config's time_scale";
    case LC_SCALE_DEFAULT:
        return "this machine could not be measured";
    default:
        return "measured";
    }
}

static void put_text(lc_str *out, const lc_time_scale *s)
{
    char text[LC_SCALE_TEXT_MAX];

    lc_str_printf(out, "CPU:     %s\n", s->cpu);
    if (s->loop_us) {
        lc_str_printf(out, "Loop:    %.1f ms, %.2f times the reference "
                      "machine's %.1f ms\n", s->loop_us / 1000.0,
                      (double)s->loop_us / (double)s->reference_us,
                      s->reference_us / 1000.0);
        lc_str_printf(out, "Measured scale: %s%s\n",
                      lc_scale_text(s->measured_percent, text),
                      s->saved ? "" : " (not kept)");
    } else {
        lc_str_puts(out, "Loop:    not measured yet\n");
    }
    if (s->percent == 0) {
        lc_str_puts(out, "In use:  measured the next time something is "
                    "judged\n");
        return;
    }
    lc_str_printf(out, "In use:  %s (%s)%s\n",
                  lc_scale_text(s->percent, text), source_text(s->source),
                  s->percent == 100 ? ", time limits as written" : "");
}

static void put_json(lc_json *j, lc_ctx *ctx, const lc_time_scale *s)
{
    lc_json_init(j, j->out);
    lc_json_begin_object(j);
    lc_json_key(j, "schema");
    lc_json_int(j, LC_JSON_SCHEMA);
    lc_json_key(j, "time_scale");
    if (s->percent)
        lc_json_double(j, s->percent / 100.0);
    else
        lc_json_null(j);
    lc_json_key(j, "time_scale_source");
    if (s->percent)
        lc_json_string(j, lc_scale_source_name(s->source));
    else
        lc_json_null(j);
    lc_json_key(j, "cpu");
    lc_json_string(j, s->cpu);
    lc_json_key(j, "loop_us");
    if (s->loop_us)
        lc_json_int(j, s->loop_us);
    else
        lc_json_null(j);
    lc_json_key(j, "reference_us");
    lc_json_int(j, s->reference_us);
    lc_json_key(j, "measured_scale");
    if (s->measured_percent)
        lc_json_double(j, s->measured_percent / 100.0);
    else
        lc_json_null(j);
    lc_json_key(j, "saved");
    lc_json_bool(j, s->saved);
    cli_json_warnings(j, ctx, NULL);
    lc_json_end_object(j);
}

int cmd_calibrate(cli *c)
{
    lc_options opt;
    lc_ctx *ctx;
    lc_time_scale s;
    lc_str out = LC_STR_INIT;
    lc_json j;
    size_t before, i;
    int rc;

    /* It needs no problem packs, so none have to be found. */
    lc_options_init(&opt);
    ctx = lc_open(&opt);
    if (!ctx)
        return cli_error(c, NULL, "out of memory");
    for (i = 0; i < lc_warning_count(ctx); i++)
        cli_warn(lc_warning_at(ctx, i));
    before = lc_warning_count(ctx);
    rc = c->show ? lc_time_scale_peek(ctx, &s) : lc_time_scale_measure(ctx, &s);
    if (rc != LC_OK) {
        rc = cli_error(c, ctx, "%s", lc_last_error(ctx));
        lc_close(ctx);
        return rc;
    }
    for (i = before; i < lc_warning_count(ctx); i++)
        cli_warn(lc_warning_at(ctx, i));
    if (c->json) {
        j.out = &out;
        put_json(&j, ctx, &s);
        rc = cli_emit_json(c, ctx, &j, &out);
    } else {
        put_text(&out, &s);
        rc = out.failed ? cli_error(c, ctx, "out of memory")
                        : cli_write(out.data, out.len);
    }
    lc_str_free(&out);
    lc_close(ctx);
    return rc;
}
