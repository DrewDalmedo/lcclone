/*
 * test_codegen.c: the code lc generates, the languages table, the
 * compiler-message parser and the embedded runtime.
 *
 * For every fixture pack in tests/fixtures/codegen, the C and C++ stubs,
 * prototypes and harnesses are compared with tests/expected/codegen/<pack>.txt.
 * Run with LC_UPDATE=1 to rewrite those files after a deliberate change
 * (then read the diff). test_build checks that what is generated compiles.
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"
#include "test.h"

#define FIXTURES "tests/fixtures/codegen"
#define EXPECTED "tests/expected/codegen"

/* One section of a golden file: the code, or the error. */
static void section(lc_str *out, lc_ctx *ctx, const char *title, int rc,
                    const lc_str *code)
{
    lc_str_printf(out, "== %s\n", title);
    if (rc == LC_OK)
        lc_str_printf(out, "%s", lc_str_cstr(code));
    else
        lc_str_printf(out, "error: %s\n", lc_last_error(ctx));
}

static void generate(lc_ctx *ctx, const lc_problem *p, lc_str *out)
{
    size_t i;

    for (i = 0; i < lc_lang_count(); i++) {
        const lc_lang *lang = lc_lang_at(i);
        lc_str code = LC_STR_INIT;
        char title[64];
        int rc;

        snprintf(title, sizeof title, "%s stub", lang->title);
        section(out, ctx, title, lc_stub(ctx, p, lang, &code), &code);
        lc_str_clear(&code);
        snprintf(title, sizeof title, "%s prototype", lang->title);
        rc = lc_prototype(ctx, p, lang, &code);
        lc_str_putc(&code, '\n');
        section(out, ctx, title, rc, &code);
        lc_str_clear(&code);
        snprintf(title, sizeof title, "%s harness", lang->title);
        section(out, ctx, title, lc_codegen_harness(ctx, p, lang, &code),
                &code);
        lc_str_free(&code);
    }
}

static void test_golden(void)
{
    lc_options opt;
    lc_ctx *ctx;
    size_t i;
    int update = getenv("LC_UPDATE") && *getenv("LC_UPDATE");

    lc_options_init(&opt);
    opt.problems_dir = FIXTURES;
    ctx = lc_open(&opt);
    CHECK_INT(lc_load(ctx), LC_OK);
    CHECK_INT(lc_warning_count(ctx), 0);
    CHECK_INT(lc_problem_count(ctx), 10);
    for (i = 0; i < lc_problem_count(ctx); i++) {
        const lc_problem *p = lc_problem_at(ctx, i);
        lc_str got = LC_STR_INIT, want = LC_STR_INIT;
        char path[256];

        snprintf(path, sizeof path, EXPECTED "/%04d-%s.txt", p->id, p->slug);
        generate(ctx, p, &got);
        CHECK(!got.failed);
        if (update) {
            FILE *f = fopen(path, "w");
            CHECK(f != NULL);
            if (f) {
                fputs(lc_str_cstr(&got), f);
                fclose(f);
            }
        } else if (lc_read_file(ctx, path, LC_MIB, &want) != LC_OK) {
            CHECK(!"missing golden file (run with LC_UPDATE=1)");
            fprintf(stderr, "    %s\n", path);
        } else if (strcmp(lc_str_cstr(&want), lc_str_cstr(&got)) != 0) {
            CHECK(!"generated code differs from the golden file");
            fprintf(stderr, "    %s; got:\n%s", path, lc_str_cstr(&got));
        }
        lc_str_free(&got);
        lc_str_free(&want);
    }
    lc_close(ctx);
}

static void test_langs(void)
{
    const lc_lang *c = lc_lang_find("c"), *cpp = lc_lang_find("cpp");

    CHECK_INT(lc_lang_count(), 2);
    CHECK(lc_lang_at(0) == c && lc_lang_at(1) == cpp);
    CHECK(lc_lang_at(2) == NULL);
    CHECK(c && c->id == LC_LANG_C && strcmp(c->ext, "c") == 0);
    CHECK(cpp && cpp->id == LC_LANG_CPP && strcmp(cpp->ext, "cpp") == 0);
    CHECK(lc_lang_find("c++") == cpp);
    CHECK(lc_lang_find("cxx") == cpp);
    CHECK(lc_lang_find("C") == NULL);
    CHECK(lc_lang_find("python") == NULL);
    CHECK(lc_lang_find(NULL) == NULL);
    CHECK_STR(c->default_std, "c11");
    CHECK_STR(cpp->default_std, "c++11");
    CHECK_STR(c->compiler, "cc");
    CHECK_STR(cpp->compiler, "c++");

    CHECK(lc_lang_for_path("solution.c") == c);
    CHECK(lc_lang_for_path("/a/b/solution.cpp") == cpp);
    CHECK(lc_lang_for_path("x.cc") == cpp);
    CHECK(lc_lang_for_path("x.cxx") == cpp);
    CHECK(lc_lang_for_path("x.h") == NULL);
    CHECK(lc_lang_for_path("x.C") == NULL);
    CHECK(lc_lang_for_path("dir.c/x") == NULL);
    CHECK(lc_lang_for_path("Makefile") == NULL);
    CHECK(lc_lang_for_path(".c") == c);
    CHECK(lc_lang_for_path(NULL) == NULL);
}

/* Parses `text` as a compiler's output and checks the messages found,
 * each written "file|line|column|severity|message|in_solution". */
static void expect_diags(const char *text, const char *const *want)
{
    lc_arena a = {NULL};
    lc_diag *d;
    size_t n, i, k = 0;
    char buf[512];

    CHECK_INT(lc_parse_diags(&a, text, strlen(text), "/w/sol.c", &d, &n),
              LC_OK);
    while (want[k])
        k++;
    CHECK_INT(n, k);
    for (i = 0; i < n && i < k; i++) {
        snprintf(buf, sizeof buf, "%s|%d|%d|%s|%s|%d", d[i].file, d[i].line,
                 d[i].column, d[i].severity, d[i].message, d[i].in_solution);
        CHECK_STR(buf, want[i]);
    }
    lc_arena_free(&a);
}

#define DIAGS(...) ((const char *const[]){__VA_ARGS__, NULL})
static const char *const no_diags[] = {NULL};

static void test_diags(void)
{
    /* clang */
    expect_diags("/w/sol.c:4:17: error: expected expression\n"
                 "    4 |         int x = ;\n"
                 "      |                 ^\n"
                 "1 error generated.\n",
                 DIAGS("/w/sol.c|4|17|error|expected expression|1"));
    /* gcc, with a note in the harness and context lines */
    expect_diags("/w/sol.c: In function 'f':\n"
                 "/w/sol.c:2:5: warning: unused variable 'x' "
                 "[-Wunused-variable]\n"
                 "lc_harness:10:50: error: too many arguments to function "
                 "'twoSum'\n"
                 "/w/sol.c:1:6: note: declared here\n"
                 "In file included from lc_prelude.h:3:\n",
                 DIAGS("/w/sol.c|2|5|warning|unused variable 'x' "
                       "[-Wunused-variable]|1",
                       "lc_harness|10|50|error|too many arguments to "
                       "function 'twoSum'|0",
                       "/w/sol.c|1|6|note|declared here|1"));
    /* No column; a fatal error; a message that itself says "error: " */
    expect_diags("/w/sol.c:3: error: oops\n"
                 "/w/sol.c:1:10: fatal error: 'nope.h' file not found\n"
                 "/w/sol.c:7:1: warning: see 'x: error: y'\n",
                 DIAGS("/w/sol.c|3|0|error|oops|1",
                       "/w/sol.c|1|10|error|'nope.h' file not found|1",
                       "/w/sol.c|7|1|warning|see 'x: error: y'|1"));
    /* Linker and driver messages have no line. */
    expect_diags("clang: error: linker command failed with exit code 1\n"
                 "collect2: error: ld returned 1 exit status\n",
                 DIAGS("clang|0|0|error|linker command failed with exit "
                       "code 1|0",
                       "collect2|0|0|error|ld returned 1 exit status|0"));
    /* Nothing that looks like a message; no trailing newline. */
    expect_diags("", no_diags);
    expect_diags("Undefined symbols for architecture arm64:\n"
                 ": error: no file\n",
                 no_diags);
    expect_diags("/w/sol.c:12:3: error: last line", DIAGS(
                 "/w/sol.c|12|3|error|last line|1"));
    /* A path with a colon and digits in it. */
    expect_diags("/w/a:1/sol.c:5:2: note: n\n",
                 DIAGS("/w/a:1/sol.c|5|2|note|n|0"));
}

static void test_runtime_files(void)
{
    static const char *const names[] = {
        "c/lc_prelude.h", "c/lc_rt.c", "c/uthash.h", "common/lc_rt_core.h",
        "cpp/lc_prelude.hpp", "cpp/lc_rt.cpp", "gen/lc_gen.h",
        "gen/lc_gen_main.c"};
    size_t i, k;

    CHECK_INT(lc_rt_nfiles, sizeof names / sizeof names[0]);
    for (i = 0; i < sizeof names / sizeof names[0]; i++) {
        const lc_rt_file *f = lc_rt_file_get(names[i]);

        CHECK(f != NULL);
        if (!f)
            continue;
        CHECK_INT(strlen(f->text), f->len);
        CHECK(f->len > 100 && f->text[f->len - 1] == '\n');
        for (k = 0; k < f->len; k++)
            if ((unsigned char)f->text[k] > 127)
                break;
        CHECK_INT(k, f->len);
    }
    CHECK(lc_rt_file_get("c/nope.h") == NULL);
}

int main(void)
{
    RUN(test_golden);
    RUN(test_langs);
    RUN(test_diags);
    RUN(test_runtime_files);
    return test_report();
}
