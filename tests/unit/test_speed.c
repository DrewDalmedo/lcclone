#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* How time limits are scaled to this machine (speed.c). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"
#include "test.h"

#define TMP "tests/tmp/speed"
#define SPEED TMP "/cache/machine-speed"
#define REF_US 6000L

static void write_file(const char *path, const char *text)
{
    FILE *f = fopen(path, "wb");

    CHECK(f != NULL);
    if (!f)
        return;
    CHECK(fputs(text, f) >= 0);
    CHECK(fclose(f) == 0);
}

static char *read_all(const char *path)
{
    static char buf[4096];
    FILE *f = fopen(path, "rb");
    size_t n;

    buf[0] = '\0';
    if (!f)
        return buf;
    n = fread(buf, 1, sizeof buf - 1, f);
    buf[n] = '\0';
    fclose(f);
    return buf;
}

/* A context with its own, empty-or-seeded cache and `config` as its
 * config text (NULL: none). */
static lc_ctx *open_ctx(const char *config)
{
    lc_options opt;

    lc_options_init(&opt);
    opt.cache_dir = TMP "/cache";
    opt.config_file = "";
    if (config) {
        write_file(TMP "/config", config);
        opt.config_file = TMP "/config";
    }
    return lc_open(&opt);
}

static void reset(void)
{
    lc_ctx *ctx;

    CHECK(system("rm -rf " TMP " && mkdir -p " TMP "/cache") == 0);
    unsetenv("LC_TIME_SCALE");
    ctx = open_ctx(NULL);
    CHECK(ctx != NULL);
    lc_close(ctx);
}

/* Keeps `us` as this CPU's time, as if it had been measured. */
static void seed(long us)
{
    lc_ctx *ctx = open_ctx(NULL);
    lc_time_scale s;
    char line[512];

    CHECK_INT(lc_time_scale_peek(ctx, &s), LC_OK);
    snprintf(line, sizeof line, "1\t%s\t%ld\t1700000000\n", s.cpu, us);
    write_file(SPEED, line);
    lc_close(ctx);
}

static void test_parse(void)
{
    static const struct {
        const char *text;
        int percent;
    } good[] = {
        {"auto", 0}, {"1", 100}, {"2", 200}, {"1.5", 150}, {"1.25", 125},
        {"0.1", 10}, {"0.25", 25}, {"100", 10000}, {"100.0", 10000},
        {"007", 700},
    };
    static const char *const bad[] = {
        "", "x", "-1", "+1", "1.", ".5", "1.234", "0", "0.09", "100.01",
        "101", "1e2", "1,5", " 1", "1 ", "AUTO", "99999999999999999999",
    };
    size_t i;
    int p;

    for (i = 0; i < sizeof good / sizeof good[0]; i++) {
        p = -1;
        CHECK_INT(lc_scale_parse(good[i].text, &p), 0);
        CHECK_INT(p, good[i].percent);
    }
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++)
        CHECK(lc_scale_parse(bad[i], &p) != 0);
}

static void test_steps(void)
{
    static const struct {
        long ratio; /* percent of the reference time */
        int scale;
    } rows[] = {
        /* Faster than the reference machine: never tighter. */
        {10, 100}, {95, 100}, {100, 100},
        /* The reference machine at full load measured up to 1.23. */
        {123, 100}, {134, 100},
        {135, 150}, {174, 150}, {175, 200}, {249, 200}, {250, 300},
        {349, 300}, {350, 400}, {400, 400}, {499, 400}, {500, 600},
        {699, 600}, {700, 800}, {999, 800}, {1000, 1200}, {1399, 1200},
        /* A wild measurement can't make the limits meaningless. */
        {1400, 1600}, {100000, 1600},
    };
    size_t i;

    for (i = 0; i < sizeof rows / sizeof rows[0]; i++)
        CHECK_INT(lc_scale_for_us(rows[i].ratio * REF_US / 100),
                  rows[i].scale);
    CHECK_INT(lc_scale_for_us(0), 100);
    CHECK_INT(lc_scale_for_us(-5), 100);
}

/* Across a step's error and 10% of noise on either side, a limit keeps
 * the room docs/authoring.md asks for: a right method at a third of the
 * limit still passes, and a slow one at three times it still fails. */
static void test_steps_keep_room(void)
{
    long ratio;

    for (ratio = 100; ratio <= 1400; ratio++) {
        double f = ratio / 100.0;
        double low = lc_scale_for_us((long)(ratio * 0.9) * REF_US / 100) / 100.0;
        double high = lc_scale_for_us((long)(ratio * 1.1) * REF_US / 100) / 100.0;

        CHECK(f / 3 < low);
        CHECK(3 * f > high);
    }
}

static void test_scale_ms(void)
{
    CHECK_INT(lc_scale_ms(50, 100), 50);
    CHECK_INT(lc_scale_ms(50, 400), 200);
    CHECK_INT(lc_scale_ms(50, 150), 75);
    CHECK_INT(lc_scale_ms(33, 150), 50);  /* rounded up */
    CHECK_INT(lc_scale_ms(1, 10), 1);
    CHECK_INT(lc_scale_ms(2000000000, 10000), 2147483647);
}

static void test_text(void)
{
    char b[LC_SCALE_TEXT_MAX];

    CHECK_STR(lc_scale_text(100, b), "1");
    CHECK_STR(lc_scale_text(1600, b), "16");
    CHECK_STR(lc_scale_text(150, b), "1.5");
    CHECK_STR(lc_scale_text(125, b), "1.25");
    CHECK_STR(lc_scale_text(10, b), "0.1");
    CHECK_STR(lc_scale_text(5, b), "0.05");
    CHECK_STR(lc_scale_source_name(LC_SCALE_MEASURED), "measured");
    CHECK_STR(lc_scale_source_name(LC_SCALE_OPTION), "option");
    CHECK_STR(lc_scale_source_name(LC_SCALE_ENV), "env");
    CHECK_STR(lc_scale_source_name(LC_SCALE_CONFIG), "config");
    CHECK_STR(lc_scale_source_name(LC_SCALE_DEFAULT), "default");
}

/* The option, then $LC_TIME_SCALE, then the config, then what's kept. */
static void test_precedence(void)
{
    lc_ctx *ctx;
    lc_time_scale s;

    reset();
    seed(4 * REF_US);
    ctx = open_ctx("time_scale = 3\n");
    CHECK_INT(lc_warning_count(ctx), 0);
    setenv("LC_TIME_SCALE", "2", 1);
    CHECK_INT(lc_time_scale_override(ctx, "1.5"), LC_OK);
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
    CHECK_INT(s.percent, 150);
    CHECK_INT(s.source, LC_SCALE_OPTION);
    /* What's kept is reported even when a setting replaces it. */
    CHECK_INT(s.measured_percent, 400);
    lc_close(ctx);

    ctx = open_ctx("time_scale = 3\n");
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
    CHECK_INT(s.percent, 200);
    CHECK_INT(s.source, LC_SCALE_ENV);
    lc_close(ctx);

    unsetenv("LC_TIME_SCALE");
    ctx = open_ctx("time_scale = 3\n");
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
    CHECK_INT(s.percent, 300);
    CHECK_INT(s.source, LC_SCALE_CONFIG);
    lc_close(ctx);

    ctx = open_ctx(NULL);
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
    CHECK_INT(s.percent, 400);
    CHECK_INT(s.source, LC_SCALE_MEASURED);
    CHECK(s.loop_us == 4 * REF_US);
    CHECK(s.reference_us == REF_US);
    CHECK(s.saved);
    lc_close(ctx);

    /* "auto" at a higher level means the measured scale, past the rest. */
    setenv("LC_TIME_SCALE", "auto", 1);
    ctx = open_ctx("time_scale = 3\n");
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
    CHECK_INT(s.percent, 400);
    CHECK_INT(s.source, LC_SCALE_MEASURED);
    lc_close(ctx);

    setenv("LC_TIME_SCALE", "2", 1);
    ctx = open_ctx(NULL);
    CHECK_INT(lc_time_scale_override(ctx, "auto"), LC_OK);
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
    CHECK_INT(s.percent, 400);
    lc_close(ctx);
    unsetenv("LC_TIME_SCALE");

    /* An empty variable is an unset one. */
    setenv("LC_TIME_SCALE", "", 1);
    ctx = open_ctx("time_scale = 3\n");
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
    CHECK_INT(s.percent, 300);
    lc_close(ctx);
    unsetenv("LC_TIME_SCALE");
}

static void test_bad_settings(void)
{
    lc_ctx *ctx;
    lc_time_scale s;

    reset();
    seed(REF_US);
    ctx = open_ctx(NULL);
    CHECK_INT(lc_time_scale_override(ctx, "fast"), LC_ERR_INVALID);
    CHECK(strstr(lc_last_error(ctx), "invalid time scale 'fast'") != NULL);
    CHECK_INT(lc_time_scale_override(ctx, NULL), LC_ERR_INVALID);
    lc_close(ctx);

    setenv("LC_TIME_SCALE", "fast", 1);
    ctx = open_ctx(NULL);
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_ERR_INVALID);
    CHECK(strstr(lc_last_error(ctx), "LC_TIME_SCALE") != NULL);
    lc_close(ctx);
    unsetenv("LC_TIME_SCALE");

    /* In the config, a bad value is a warning, and lc goes on. */
    ctx = open_ctx("time_scale = fast\n");
    CHECK_INT(lc_warning_count(ctx), 1);
    CHECK(strstr(lc_warning_at(ctx, 0), "time_scale") != NULL);
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
    CHECK_INT(s.percent, 100);
    CHECK_INT(s.source, LC_SCALE_MEASURED);
    lc_close(ctx);
}

/* Judging uses what is kept and measures nothing: the verdicts of one
 * machine can't change from run to run. */
static void test_kept_time_is_used(void)
{
    lc_ctx *ctx;
    lc_time_scale s;
    char before[1024];
    int i;

    reset();
    seed(4 * REF_US); /* no machine here is really this one */
    strcpy(before, read_all(SPEED));
    for (i = 0; i < 3; i++) {
        ctx = open_ctx(NULL);
        CHECK_INT(lc_time_scale_peek(ctx, &s), LC_OK);
        CHECK_INT(s.percent, 400);
        CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
        CHECK_INT(s.percent, 400);
        CHECK(s.loop_us == 4 * REF_US);
        lc_close(ctx);
    }
    CHECK_STR(read_all(SPEED), before);
}

static void test_measure(void)
{
    lc_ctx *ctx;
    lc_time_scale s, again;
    long us;

    reset();
    ctx = open_ctx(NULL);
    CHECK_INT(lc_time_scale_peek(ctx, &s), LC_OK);
    CHECK_INT(s.percent, 0);
    CHECK(s.loop_us == 0);
    CHECK(s.cpu != NULL && s.cpu[0] != '\0');
    CHECK(fopen(SPEED, "rb") == NULL);
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
    CHECK(s.loop_us > 0);
    CHECK(s.saved);
    CHECK(s.percent >= 100 && s.percent <= 1600);
    CHECK_INT(s.source, LC_SCALE_MEASURED);
    us = s.loop_us;
    lc_close(ctx);

    /* The next process reads it. */
    ctx = open_ctx(NULL);
    CHECK_INT(lc_time_scale_get(ctx, &again), LC_OK);
    CHECK(again.loop_us == us);
    CHECK_INT(again.percent, lc_scale_for_us(us));
    lc_close(ctx);

    /* Measuring again replaces it, and a setting still wins. */
    seed(5 * REF_US);
    setenv("LC_TIME_SCALE", "3", 1);
    ctx = open_ctx(NULL);
    CHECK_INT(lc_time_scale_measure(ctx, &s), LC_OK);
    CHECK(s.loop_us != 5 * REF_US);
    CHECK_INT(s.percent, 300);
    CHECK_INT(s.source, LC_SCALE_ENV);
    CHECK_INT(s.measured_percent, lc_scale_for_us(s.loop_us));
    lc_close(ctx);
    unsetenv("LC_TIME_SCALE");
}

/* The file: other CPUs' lines stay, an old version's and broken ones go. */
static void test_file(void)
{
    lc_ctx *ctx;
    lc_time_scale s;
    char text[1024], *now;

    reset();
    ctx = open_ctx(NULL);
    CHECK_INT(lc_time_scale_peek(ctx, &s), LC_OK);
    snprintf(text, sizeof text,
             "# a comment\n"
             "1\tother: CPU\t12345\t1700000000\n"
             "999\t%s\t77\t1700000000\n"
             "1\t%s\tnot-a-number\t1700000000\n"
             "1\t%s\t0\t1700000000\n"
             "garbage\n",
             s.cpu, s.cpu, s.cpu);
    lc_close(ctx);
    write_file(SPEED, text);

    ctx = open_ctx(NULL);
    CHECK_INT(lc_time_scale_peek(ctx, &s), LC_OK);
    CHECK(s.loop_us == 0); /* no line it can use */
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
    CHECK(s.loop_us > 0);
    lc_close(ctx);
    now = read_all(SPEED);
    CHECK(strstr(now, "1\tother: CPU\t12345\t1700000000\n") != NULL);
    CHECK(strstr(now, "999\t") == NULL);
    CHECK(strstr(now, "not-a-number") == NULL);
    CHECK(strstr(now, "garbage") == NULL);

    /* The CPU's name must match whole. */
    reset();
    ctx = open_ctx(NULL);
    CHECK_INT(lc_time_scale_peek(ctx, &s), LC_OK);
    snprintf(text, sizeof text, "1\t%s Pro\t12345\t1700000000\n", s.cpu);
    lc_close(ctx);
    write_file(SPEED, text);
    ctx = open_ctx(NULL);
    CHECK_INT(lc_time_scale_peek(ctx, &s), LC_OK);
    CHECK(s.loop_us == 0);
    lc_close(ctx);
}

/* A cache that can't be written: the scale still works, with a warning. */
static void test_no_cache(void)
{
    lc_options opt;
    lc_ctx *ctx;
    lc_time_scale s;

    reset();
    write_file(TMP "/file", "not a directory\n");
    lc_options_init(&opt);
    opt.cache_dir = TMP "/file/cache";
    opt.config_file = "";
    ctx = lc_open(&opt);
    CHECK_INT(lc_time_scale_get(ctx, &s), LC_OK);
    CHECK(s.loop_us > 0);
    CHECK(!s.saved);
    CHECK(s.percent >= 100);
    CHECK_INT(lc_warning_count(ctx), 1);
    CHECK(strstr(lc_warning_at(ctx, 0), "measure it again") != NULL);
    lc_close(ctx);
}

static void test_message(void)
{
    lc_limits l;
    lc_run_result r;
    lc_str why = LC_STR_INIT;

    lc_run_result_init(&r);
    r.exited = 1;
    r.cpu_ms = 250;
    lc_limits_init(&l, 200, 256);
    CHECK_INT(lc_classify(&l, &r, &why), LC_TIME_LIMIT_EXCEEDED);
    CHECK_STR(lc_str_cstr(&why),
              "used 250 ms of CPU time; the limit is 200 ms");
    l.base_ms = 100;
    l.scale_percent = 200;
    CHECK_INT(lc_classify(&l, &r, &why), LC_TIME_LIMIT_EXCEEDED);
    CHECK_STR(lc_str_cstr(&why), "used 250 ms of CPU time; the limit is "
              "200 ms (100 ms x 2 for this machine)");
    r.cpu_ms = 3;
    r.timed_out = 1;
    r.wall_ms = 1500;
    CHECK_INT(lc_classify(&l, &r, &why), LC_TIME_LIMIT_EXCEEDED);
    CHECK_STR(lc_str_cstr(&why), "was still running after 1500 ms (it used "
              "3 ms of CPU time; the limit is 200 ms (100 ms x 2 for this "
              "machine))");
    lc_str_free(&why);
    lc_run_result_free(&r);
}

int main(void)
{
    if (system("mkdir -p " TMP) != 0)
        return 1;
    RUN(test_parse);
    RUN(test_steps);
    RUN(test_steps_keep_room);
    RUN(test_scale_ms);
    RUN(test_text);
    RUN(test_precedence);
    RUN(test_bad_settings);
    RUN(test_kept_time_is_used);
    RUN(test_measure);
    RUN(test_file);
    RUN(test_no_cache);
    RUN(test_message);
    return test_report();
}
