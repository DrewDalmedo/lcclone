#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* sysctlbyname, for the CPU's name. */
#ifndef _DARWIN_C_SOURCE
#define _DARWIN_C_SOURCE
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * speed.c: how much time limits are scaled on this machine
 * (docs/judging.md, "Slower machines").
 *
 * A pack's time_limit_ms is written for the reference machine. lc times a
 * fixed loop once per CPU, keeps the result in <cache>/machine-speed, and
 * turns the ratio to the reference machine's time into one of a few coarse
 * steps. What keeps verdicts stable:
 *
 *   - judging never measures again once a time is kept for this CPU;
 *   - the time is the best of many short rounds, since noise only ever
 *     makes a round slower;
 *   - the steps are wide, so that what noise is left rarely changes one.
 */

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

#if defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#include "internal.h"

/* Changes when the loop does: times kept by another version are dropped. */
#define SPEED_VERSION 1
/*
 * One round's best time on the reference machine, an Apple M4: 5.7 ms with
 * lc built by Apple clang, 6.4 ms by gcc 16. This is about the middle.
 */
#define REFERENCE_US 6000L

#define SPEED_FILE "machine-speed"
#define SPEED_LOCK "machine-speed.lock"

/* 128 KiB of table: past the first-level cache of any CPU, inside the
 * second. */
#define TABLE_SIZE (1u << 15)
#define LOOP_STEPS 2000000UL
#define MAX_ROUNDS 15
#define MIN_ROUNDS 5
/* On something very slow (an emulator), stop after this much CPU time. */
#define MAX_CPU_US 1500000L
/* Between rounds, so that the scheduler may place the next one on another
 * core: on a CPU with fast and slow cores, the best time is the fast one's. */
#define ROUND_GAP_NS 1000000L

/*
 * The scale for a ratio to the reference time, in percent: the first row
 * whose `below` is past the ratio. The steps are 1.33 to 1.5 times apart,
 * so a scale is at most 1.25 times off the ratio it stands for (1.35 in
 * the first row, which is wider because the reference machine itself
 * measures up to 1.23 with every core busy). A pack's limit leaves three
 * times the room on either side of it (docs/authoring.md), so that error
 * doesn't change the verdict on a method LeetCode clearly accepts or
 * rejects.
 */
static const struct {
    int below;
    int scale;
} steps[] = {
    {135, 100}, {175, 150}, {250, 200}, {350, 300},
    {500, 400}, {700, 600}, {1000, 800}, {1400, 1200},
};
#define TOP_SCALE 1600

uint64_t lc_speed_loop(const uint32_t *table, uint32_t mask, uint64_t x,
                       unsigned long n);

int lc_scale_for_us(long us)
{
    long ratio;
    size_t i;

    if (us <= 0)
        return 100;
    ratio = us > LONG_MAX / 100 ? LONG_MAX : us * 100 / REFERENCE_US;
    for (i = 0; i < sizeof steps / sizeof steps[0]; i++)
        if (ratio < steps[i].below)
            return steps[i].scale;
    return TOP_SCALE;
}

int lc_scale_ms(int ms, int percent)
{
    long long scaled = ((long long)ms * percent + 99) / 100;

    if (scaled < 1)
        return 1;
    return scaled > INT_MAX ? INT_MAX : (int)scaled;
}

int lc_scale_parse(const char *s, int *percent)
{
    long whole = 0, frac = 0;
    int digits = 0;

    if (strcmp(s, "auto") == 0) {
        *percent = 0;
        return 0;
    }
    /* By hand: strtod would follow the locale's decimal point. */
    if (*s < '0' || *s > '9')
        return -1;
    for (; *s >= '0' && *s <= '9'; s++) {
        whole = whole * 10 + (*s - '0');
        if (whole > 100)
            return -1;
    }
    if (*s == '.') {
        s++;
        if (*s < '0' || *s > '9')
            return -1;
        for (; *s >= '0' && *s <= '9'; s++, digits++) {
            if (digits >= 2)
                return -1;
            frac = frac * 10 + (*s - '0');
        }
        if (digits == 1)
            frac *= 10;
    }
    if (*s)
        return -1;
    whole = whole * 100 + frac;
    if (whole < LC_SCALE_MIN || whole > LC_SCALE_MAX)
        return -1;
    *percent = (int)whole;
    return 0;
}

const char *lc_scale_text(int percent, char *buf)
{
    if (percent % 100 == 0)
        snprintf(buf, LC_SCALE_TEXT_MAX, "%d", percent / 100);
    else if (percent % 10 == 0)
        snprintf(buf, LC_SCALE_TEXT_MAX, "%d.%d", percent / 100,
                 percent / 10 % 10);
    else
        snprintf(buf, LC_SCALE_TEXT_MAX, "%d.%02d", percent / 100,
                 percent % 100);
    return buf;
}

const char *lc_scale_source_name(lc_scale_source s)
{
    switch (s) {
    case LC_SCALE_OPTION:
        return "option";
    case LC_SCALE_ENV:
        return "env";
    case LC_SCALE_CONFIG:
        return "config";
    case LC_SCALE_DEFAULT:
        return "default";
    default:
        return "measured";
    }
}

/* ------------------------------------------------------------------ */
/* Measuring                                                           */
/* ------------------------------------------------------------------ */

/* This thread's CPU time, in microseconds; -1 when it can't be read. */
static long long cpu_us(void)
{
#ifdef CLOCK_THREAD_CPUTIME_ID
    struct timespec ts;

    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts) == 0)
        return (long long)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
#endif
    {
        struct rusage ru;

        if (getrusage(RUSAGE_SELF, &ru) != 0)
            return -1;
        return ((long long)ru.ru_utime.tv_sec + ru.ru_stime.tv_sec) * 1000000 +
               ru.ru_utime.tv_usec + ru.ru_stime.tv_usec;
    }
}

/* The loop's best time over its rounds, or -1 (no memory, no clock). */
static long measure_loop(void)
{
    uint32_t *table = malloc(TABLE_SIZE * sizeof *table);
    uint64_t x = 0x2545F4914F6CDD1Dull;
    volatile uint64_t sink = 0;
    long long best = -1, spent = 0;
    uint32_t i;
    int round;

    if (!table)
        return -1;
    /* Sattolo's shuffle: one cycle through the whole table. */
    for (i = 0; i < TABLE_SIZE; i++)
        table[i] = i;
    for (i = TABLE_SIZE - 1; i > 0; i--) {
        uint32_t j, tmp;

        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        j = (uint32_t)(x % i);
        tmp = table[i];
        table[i] = table[j];
        table[j] = tmp;
    }
    for (round = 0; round < MAX_ROUNDS; round++) {
        struct timespec gap = {0, ROUND_GAP_NS};
        long long start = cpu_us(), took;

        sink += lc_speed_loop(table, TABLE_SIZE - 1, x + (uint64_t)round,
                              LOOP_STEPS);
        took = cpu_us() - start;
        if (start < 0 || took <= 0) {
            free(table);
            return -1;
        }
        if (best < 0 || took < best)
            best = took;
        spent += took;
        if (round + 1 >= MIN_ROUNDS && spent > MAX_CPU_US)
            break;
        nanosleep(&gap, NULL);
    }
    free(table);
    (void)sink;
    return best > LONG_MAX ? LONG_MAX : (long)best;
}

/* ------------------------------------------------------------------ */
/* Which CPU this is                                                   */
/* ------------------------------------------------------------------ */

#if !defined(__APPLE__)
/* The first 64 KiB of /proc/cpuinfo: one CPU's lines are all it takes, and
 * the whole file of a big server can be megabytes. */
static int read_cpuinfo(lc_str *out)
{
    char buf[65536];
    size_t got = 0;
    int fd;

    do
        fd = open("/proc/cpuinfo", O_RDONLY | O_CLOEXEC);
    while (fd < 0 && errno == EINTR);
    if (fd < 0)
        return -1;
    while (got < sizeof buf) {
        ssize_t n = read(fd, buf + got, sizeof buf - got);

        if (n < 0 && errno == EINTR)
            continue;
        if (n <= 0)
            break;
        got += (size_t)n;
    }
    close(fd);
    return lc_str_append(out, buf, got) == LC_OK ? 0 : -1;
}

/* The value of the first "KEY : value" line of /proc/cpuinfo, in `out`. */
static int cpuinfo_value(const char *text, const char *key, lc_str *out)
{
    size_t klen = strlen(key);
    const char *p = text;

    while (*p) {
        const char *nl = strchr(p, '\n');
        const char *end = nl ? nl : p + strlen(p);

        if ((size_t)(end - p) > klen && strncmp(p, key, klen) == 0 &&
            (p[klen] == ' ' || p[klen] == '\t' || p[klen] == ':')) {
            const char *v = memchr(p, ':', (size_t)(end - p));

            if (v) {
                for (v++; v < end && (*v == ' ' || *v == '\t'); v++)
                    ;
                if (v < end) {
                    lc_str_append(out, v, (size_t)(end - v));
                    return 1;
                }
            }
        }
        p = nl ? nl + 1 : end;
    }
    return 0;
}
#endif

/*
 * What identifies this CPU: its architecture and model, such as "arm64:
 * Apple M4". Not the host name, which changes on a laptop from one network
 * to the next. A home directory shared by machines keeps one line for each
 * kind of CPU. In `ctx`'s arena; NULL when out of memory.
 */
static const char *cpu_name(lc_ctx *ctx)
{
    lc_str s = LC_STR_INIT;
    struct utsname u;
    char *out;
    size_t i;

    if (uname(&u) == 0)
        lc_str_puts(&s, u.machine);
    else
        lc_str_puts(&s, "unknown");
#if defined(__APPLE__)
    {
        char brand[256];
        size_t n = sizeof brand;

        if (sysctlbyname("machdep.cpu.brand_string", brand, &n, NULL, 0) == 0
            && n > 1) {
            lc_str_puts(&s, ": ");
            lc_str_append(&s, brand, strnlen(brand, n));
        }
    }
#else
    {
        lc_str info = LC_STR_INIT;

        if (read_cpuinfo(&info) == 0) {
            size_t mark = s.len;

            lc_str_puts(&s, ": ");
            if (!cpuinfo_value(lc_str_cstr(&info), "model name", &s)) {
                /* Most arm64 kernels give only the part's numbers. */
                int impl = cpuinfo_value(lc_str_cstr(&info),
                                         "CPU implementer", &s);

                if (impl)
                    lc_str_putc(&s, '/');
                if (!cpuinfo_value(lc_str_cstr(&info), "CPU part", &s) &&
                    !impl) {
                    s.len = mark;
                    if (s.data)
                        s.data[mark] = '\0';
                }
            }
        }
        lc_str_free(&info);
    }
#endif
    /* It goes in a tab-separated line. */
    for (i = 0; i < s.len; i++)
        if (s.data[i] == '\t' || s.data[i] == '\n' || s.data[i] == '\r')
            s.data[i] = ' ';
    out = s.failed ? NULL : lc_arena_strndup(&ctx->arena, lc_str_cstr(&s),
                                             s.len);
    lc_str_free(&s);
    return out;
}

/* ------------------------------------------------------------------ */
/* The file                                                            */
/* ------------------------------------------------------------------ */

/*
 * Each line is "<version>\t<cpu>\t<microseconds>\t<when>". Finds this CPU's
 * time; *us is 0 when there's none, or the line can't be used.
 */
static void find_time(const lc_str *text, const char *cpu, long *us)
{
    const char *p = lc_str_cstr(text), *end = p + text->len;
    char version[16];
    int vn = snprintf(version, sizeof version, "%d\t", SPEED_VERSION);
    size_t clen = strlen(cpu);

    *us = 0;
    while (p < end) {
        const char *nl = memchr(p, '\n', (size_t)(end - p));
        const char *le = nl ? nl : end;

        if ((size_t)(le - p) > (size_t)vn + clen + 1 &&
            memcmp(p, version, (size_t)vn) == 0 &&
            memcmp(p + vn, cpu, clen) == 0 && p[(size_t)vn + clen] == '\t') {
            const char *f = p + (size_t)vn + clen + 1;
            long v = 0;

            for (; f < le && *f >= '0' && *f <= '9'; f++) {
                if (v > (LONG_MAX - 9) / 10) {
                    v = 0;
                    break;
                }
                v = v * 10 + (*f - '0');
            }
            if (f < le && *f == '\t' && v > 0)
                *us = v;
        }
        p = le + (nl ? 1 : 0);
    }
}

/* Writes the file again: the other CPUs' lines of this version, and this
 * CPU's new one. */
static int save_time(lc_ctx *ctx, const char *path, const lc_str *old,
                     const char *cpu, long us)
{
    lc_str out = LC_STR_INIT;
    const char *p = lc_str_cstr(old), *end = p + old->len;
    char version[16];
    int vn = snprintf(version, sizeof version, "%d\t", SPEED_VERSION), rc;
    size_t clen = strlen(cpu);

    lc_str_puts(&out, "# How fast this machine runs lc's calibration loop "
                "('lc calibrate' measures it again).\n");
    while (p < end) {
        const char *nl = memchr(p, '\n', (size_t)(end - p));
        const char *le = nl ? nl : end;
        size_t n = (size_t)(le - p);

        if (n > (size_t)vn && memcmp(p, version, (size_t)vn) == 0 &&
            !(n > (size_t)vn + clen && memcmp(p + vn, cpu, clen) == 0 &&
              p[(size_t)vn + clen] == '\t')) {
            lc_str_append(&out, p, n);
            lc_str_putc(&out, '\n');
        }
        p = le + (nl ? 1 : 0);
    }
    lc_str_printf(&out, "%d\t%s\t%ld\t%lld\n", SPEED_VERSION, cpu, us,
                  (long long)time(NULL));
    if (out.failed) {
        lc_str_free(&out);
        return lc_oom(ctx);
    }
    rc = lc_write_file_atomic(ctx, path, out.data, out.len);
    lc_str_free(&out);
    return rc;
}

/* Takes the lock around measuring, so that two lc processes starting at
 * once don't load the machine for each other and then disagree. -1 when
 * there is no lock to be had, which is not an error. */
static int lock_speed(const char *path)
{
    struct flock fl;
    int fd;

    do
        fd = open(path, O_RDWR | O_CREAT | O_CLOEXEC, 0666);
    while (fd < 0 && errno == EINTR);
    if (fd < 0)
        return -1;
    memset(&fl, 0, sizeof fl);
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    while (fcntl(fd, F_SETLKW, &fl) != 0) {
        if (errno != EINTR) {
            close(fd);
            return -1;
        }
    }
    return fd;
}

static int read_times(lc_ctx *ctx, const char *path, lc_str *out)
{
    int rc;

    lc_str_clear(out);
    if (!path)
        return LC_OK;
    rc = lc_read_file(ctx, path, LC_MIB, out);
    if (rc == LC_ERR_NOMEM)
        return rc;
    if (rc != LC_OK)
        lc_str_clear(out); /* missing or unreadable: measure afresh */
    return LC_OK;
}

/*
 * Measures, keeps the time and sets *us. A file that can't be written only
 * means measuring again next time, with a warning. *us stays 0 when it
 * can't be measured.
 */
static int measure_and_save(lc_ctx *ctx, const char *dir, const char *cpu,
                            int again, long *us, int *saved)
{
    lc_str path = LC_STR_INIT, lock = LC_STR_INIT, text = LC_STR_INIT;
    int rc = LC_OK, fd = -1;
    long t;

    *saved = 0;
    if (dir) {
        rc = lc_mkdir_p(ctx, dir, 0777);
        if (rc == LC_ERR_NOMEM)
            return rc;
        if (rc != LC_OK)
            dir = NULL;
        rc = LC_OK;
    }
    if (dir) {
        lc_str_printf(&path, "%s/" SPEED_FILE, dir);
        lc_str_printf(&lock, "%s/" SPEED_LOCK, dir);
        if (path.failed || lock.failed) {
            rc = lc_oom(ctx);
            goto done;
        }
        fd = lock_speed(lock.data);
        rc = read_times(ctx, path.data, &text);
        if (rc != LC_OK)
            goto done;
        if (!again) {
            /* Another lc may have measured while this one waited. */
            find_time(&text, cpu, us);
            if (*us) {
                *saved = 1;
                goto done;
            }
        }
    }
    t = measure_loop();
    if (t <= 0)
        goto done;
    *us = t;
    if (dir) {
        rc = save_time(ctx, path.data, &text, cpu, t);
        if (rc == LC_ERR_NOMEM)
            goto done;
        if (rc != LC_OK) {
            rc = lc_add_warning(ctx, "this machine's speed was not kept, so "
                                "lc will measure it again: %s",
                                lc_last_error(ctx));
            if (rc != LC_OK)
                goto done;
        } else {
            *saved = 1;
        }
    } else {
        rc = lc_add_warning(ctx, "no cache directory to keep this machine's "
                            "speed in, so lc will measure it again");
    }
done:
    if (fd >= 0)
        close(fd);
    lc_str_free(&path);
    lc_str_free(&lock);
    lc_str_free(&text);
    return rc;
}

/* ------------------------------------------------------------------ */
/* The scale                                                           */
/* ------------------------------------------------------------------ */

int lc_time_scale_override(lc_ctx *ctx, const char *value)
{
    int percent;

    if (!value || lc_scale_parse(value, &percent) != 0)
        return lc_set_error(ctx, LC_ERR_INVALID, "invalid time scale '%s' "
                            "(expected auto, or a number from 0.1 to 100 "
                            "with at most two decimals)", value ? value : "");
    ctx->scale_given = 1;
    ctx->scale_given_percent = percent;
    ctx->scale_known = 0;
    return LC_OK;
}

/*
 * What the settings ask for: *percent is 0 for the measured scale. The
 * most specific wins: the option, $LC_TIME_SCALE, the config.
 */
static int asked_scale(lc_ctx *ctx, int *percent, lc_scale_source *source)
{
    const char *v;

    *percent = 0;
    *source = LC_SCALE_MEASURED;
    if (ctx->scale_given) {
        *percent = ctx->scale_given_percent;
        *source = LC_SCALE_OPTION;
    } else if ((v = lc_getenv("LC_TIME_SCALE")) != NULL) {
        if (lc_scale_parse(v, percent) != 0)
            return lc_set_error(ctx, LC_ERR_INVALID, "LC_TIME_SCALE: invalid "
                                "value '%s' (expected auto, or a number from "
                                "0.1 to 100 with at most two decimals)", v);
        *source = LC_SCALE_ENV;
    } else if ((v = lc_config_get(ctx, "time_scale")) != NULL) {
        /* The config reader checked it. */
        if (lc_scale_parse(v, percent) != 0)
            *percent = 0;
        *source = LC_SCALE_CONFIG;
    }
    if (*percent == 0)
        *source = LC_SCALE_MEASURED;
    return LC_OK;
}

enum { RESOLVE_PEEK, RESOLVE_USE, RESOLVE_MEASURE };

static int resolve(lc_ctx *ctx, int mode, lc_time_scale *out)
{
    lc_str path = LC_STR_INIT, text = LC_STR_INIT;
    const char *dir, *cpu;
    lc_scale_source source;
    int rc, asked, saved = 0;
    long us = 0;

    memset(out, 0, sizeof *out);
    rc = asked_scale(ctx, &asked, &source);
    if (rc != LC_OK)
        return rc;
    cpu = cpu_name(ctx);
    if (!cpu)
        return lc_oom(ctx);
    dir = lc_cache_dir(ctx);
    if (!dir && ctx->err_nomem)
        return LC_ERR_NOMEM;
    if (dir) {
        if (lc_str_printf(&path, "%s/" SPEED_FILE, dir) != LC_OK) {
            rc = lc_oom(ctx);
            goto done;
        }
        rc = read_times(ctx, path.data, &text);
        if (rc != LC_OK)
            goto done;
        find_time(&text, cpu, &us);
        saved = us != 0;
    }
    if (mode == RESOLVE_MEASURE || (mode == RESOLVE_USE && !asked && !us)) {
        if (mode == RESOLVE_MEASURE)
            us = 0;
        rc = measure_and_save(ctx, dir, cpu, mode == RESOLVE_MEASURE, &us,
                              &saved);
        if (rc != LC_OK)
            goto done;
    }
    out->cpu = cpu;
    out->loop_us = us;
    out->reference_us = REFERENCE_US;
    out->measured_percent = us ? lc_scale_for_us(us) : 0;
    out->saved = saved;
    if (asked) {
        out->percent = asked;
        out->source = source;
    } else if (us) {
        out->percent = out->measured_percent;
        out->source = LC_SCALE_MEASURED;
    } else if (mode == RESOLVE_PEEK) {
        out->percent = 0;
        out->source = LC_SCALE_MEASURED;
    } else {
        out->percent = 100;
        out->source = LC_SCALE_DEFAULT;
    }
done:
    lc_str_free(&path);
    lc_str_free(&text);
    return rc;
}

int lc_time_scale_get(lc_ctx *ctx, lc_time_scale *out)
{
    int rc;

    if (!ctx->scale_known) {
        rc = resolve(ctx, RESOLVE_USE, &ctx->scale);
        if (rc != LC_OK)
            return rc;
        ctx->scale_known = 1;
    }
    *out = ctx->scale;
    return LC_OK;
}

int lc_time_scale_peek(lc_ctx *ctx, lc_time_scale *out)
{
    if (ctx->scale_known) {
        *out = ctx->scale;
        return LC_OK;
    }
    return resolve(ctx, RESOLVE_PEEK, out);
}

int lc_time_scale_measure(lc_ctx *ctx, lc_time_scale *out)
{
    int rc = resolve(ctx, RESOLVE_MEASURE, &ctx->scale);

    if (rc != LC_OK) {
        ctx->scale_known = 0;
        return rc;
    }
    ctx->scale_known = 1;
    *out = ctx->scale;
    return LC_OK;
}
