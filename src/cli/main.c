#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/*
 * main.c: the lc command line. Parses arguments, runs a command, and
 * reports errors. Help, version and the shared error and JSON helpers live
 * here too.
 */

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"

static int cmd_help(cli *c);
static int cmd_version(cli *c);

/* The general help: main_help_head, the command list, main_help_tail. */
static const char main_help_head[] =
    "usage: lc <command> [options] [arguments]\n"
    "\n"
    "lc is an offline LeetCode-style practice tool. It lists problems and\n"
    "shows their statements and examples in the terminal.\n"
    "\n"
    "Commands:\n";

static const char main_help_tail[] =
    "\n"
    "Run 'lc help <command>' for the options of a command. Every command\n"
    "takes --json for machine-readable output (see docs/json.md).\n"
    "\n"
    "Environment:\n"
    "  LC_PROBLEMS  directory of problem packs (default:\n"
    "               ~/.local/share/lcclone/problems, then the installed ones)\n"
    "  PAGER        pager for 'lc show' (default: less, then more)\n"
    "  NO_COLOR     if set and not empty, --color=auto never uses colour\n"
    "  COLUMNS      width to wrap text at (default: the terminal's, or 80)\n"
    "\n"
    "Exit status: 0 on success, 3 on a usage error, an unknown problem or\n"
    "any other failure.\n";

static const cli_cmd commands[] = {
    {"list", "List problems, filtered by difficulty and tags",
     "lc list [--difficulty LIST] [--tag TAG]... [--json] [--color=WHEN]",
     "List problems in ID order: a table of ID, title, difficulty and tags.\n"
     "Prints nothing when no problem matches.\n"
     "\n"
     "Options:\n"
     "  --difficulty LIST  only these difficulties: a comma-separated list\n"
     "                     of easy, medium and hard\n"
     "  --tag TAG          only problems with this tag; repeat it to require\n"
     "                     several tags\n"
     "  --json             print JSON instead of a table\n"
     "  --color=WHEN       colour the output: auto (the default: only on a\n"
     "                     terminal, without NO_COLOR, when TERM is not\n"
     "                     dumb), always or never\n"
     "  -h, --help         show this help\n",
     CLI_OPT_COLOR | CLI_OPT_DIFFICULTY | CLI_OPT_TAG, 0, 0, NULL, cmd_list},
    {"show", "Show a problem's statement and examples",
     "lc show <problem> [--json] [--no-pager] [--color=WHEN]",
     "Show a problem's statement and examples. <problem> is an ID (1 or\n"
     "0001), a slug (two-sum) or a pack directory name (0001-two-sum).\n"
     "\n"
     "On a terminal the text goes through $PAGER (default: less, then more).\n"
     "Set PAGER=cat or use --no-pager to print it directly.\n"
     "\n"
     "Options:\n"
     "  --json             print JSON: the problem's details, statement,\n"
     "                     parsed statement blocks and examples\n"
     "  --no-pager         never use a pager\n"
     "  --color=WHEN       colour the output: auto (the default), always or\n"
     "                     never\n"
     "  -h, --help         show this help\n",
     CLI_OPT_COLOR | CLI_OPT_NO_PAGER, 1, 1,
     "missing problem (an ID, slug or directory name)", cmd_show},
    {"help", "Show help for lc or for one command",
     "lc help [<command>] [--json]",
     "Show help for lc, or for one command.\n"
     "\n"
     "Options:\n"
     "  --json             print the commands as JSON\n"
     "  -h, --help         show this help\n",
     0, 0, 1, NULL, cmd_help},
    {"version", "Print the version", "lc version [--json]",
     "Print the version of lc.\n"
     "\n"
     "Options:\n"
     "  --json             print JSON\n"
     "  -h, --help         show this help\n",
     0, 0, 0, NULL, cmd_version},
};

#define NCOMMANDS (sizeof commands / sizeof commands[0])

static const cli_cmd *find_command(const char *name)
{
    size_t i;

    for (i = 0; i < NCOMMANDS; i++)
        if (strcmp(commands[i].name, name) == 0)
            return &commands[i];
    return NULL;
}

/* ------------------------------------------------------------------ */
/* Errors, warnings and JSON output                                    */
/* ------------------------------------------------------------------ */

void cli_warn(const char *msg)
{
    fprintf(stderr, "lc: warning: %s\n", msg);
}

void cli_json_warnings(lc_json *j, const lc_ctx *ctx, const char *extra)
{
    size_t i, n = ctx ? lc_warning_count(ctx) : 0;

    lc_json_key(j, "warnings");
    lc_json_begin_array(j);
    for (i = 0; i < n; i++)
        lc_json_string(j, lc_warning_at(ctx, i));
    if (extra)
        lc_json_string(j, extra);
    lc_json_end_array(j);
}

static int report(cli *c, lc_ctx *ctx, int hint, const char *fmt, va_list ap)
{
    lc_str msg = LC_STR_INIT, out = LC_STR_INIT;
    const char *text;
    lc_json j;

    lc_str_vprintf(&msg, fmt, ap);
    text = msg.failed ? "out of memory" : lc_str_cstr(&msg);
    fprintf(stderr, "lc: %s\n", text);
    if (hint) {
        /* Errors in 'lc help' point at the general help. */
        if (c->cmd && strcmp(c->cmd->name, "help") != 0)
            fprintf(stderr, "Run 'lc help %s'.\n", c->cmd->name);
        else
            fprintf(stderr, "Run 'lc help'.\n");
    }
    if (c->json) {
        lc_json_init(&j, &out);
        lc_json_begin_object(&j);
        lc_json_key(&j, "schema");
        lc_json_int(&j, LC_JSON_SCHEMA);
        lc_json_key(&j, "error");
        lc_json_begin_object(&j);
        lc_json_key(&j, "message");
        lc_json_string(&j, text);
        lc_json_end_object(&j);
        cli_json_warnings(&j, ctx, NULL);
        lc_json_end_object(&j);
        if (lc_json_finish(&j) == LC_OK && lc_str_putc(&out, '\n') == LC_OK)
            cli_write(out.data, out.len);
    }
    lc_str_free(&out);
    lc_str_free(&msg);
    return CLI_EXIT_ERROR;
}

int cli_error(cli *c, lc_ctx *ctx, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    report(c, ctx, 0, fmt, ap);
    va_end(ap);
    return CLI_EXIT_ERROR;
}

int cli_usage(cli *c, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    report(c, NULL, 1, fmt, ap);
    va_end(ap);
    return CLI_EXIT_ERROR;
}

int cli_load(cli *c, lc_ctx **out)
{
    lc_ctx *ctx = lc_open(NULL);
    size_t i;
    int rc;

    *out = NULL;
    if (!ctx)
        return cli_error(c, NULL, "out of memory");
    rc = lc_load(ctx);
    for (i = 0; i < lc_warning_count(ctx); i++)
        cli_warn(lc_warning_at(ctx, i));
    if (rc != LC_OK) {
        rc = cli_error(c, ctx, "%s", lc_last_error(ctx));
        lc_close(ctx);
        return rc;
    }
    *out = ctx;
    return CLI_EXIT_OK;
}

int cli_emit_json(cli *c, lc_ctx *ctx, lc_json *j, lc_str *out)
{
    int rc = lc_json_finish(j);

    if (rc == LC_OK)
        rc = lc_str_putc(out, '\n');
    if (rc == LC_ERR_NOMEM)
        return cli_error(c, ctx, "out of memory");
    if (rc != LC_OK)
        return cli_error(c, ctx, "internal error: malformed JSON output");
    return cli_write(out->data, out->len);
}

/* ------------------------------------------------------------------ */
/* help and version                                                    */
/* ------------------------------------------------------------------ */

static void help_json_command(lc_json *j, const cli_cmd *cmd)
{
    lc_json_begin_object(j);
    lc_json_key(j, "name");
    lc_json_string(j, cmd->name);
    lc_json_key(j, "summary");
    lc_json_string(j, cmd->summary);
    lc_json_key(j, "usage");
    lc_json_string(j, cmd->usage);
    lc_json_end_object(j);
}

/* The general help (cmd == NULL) or a command's, as text. */
static void help_text(lc_str *out, const cli_cmd *cmd)
{
    size_t i;

    if (cmd) {
        lc_str_printf(out, "usage: %s\n\n%s", cmd->usage, cmd->help);
        return;
    }
    lc_str_puts(out, main_help_head);
    for (i = 0; i < NCOMMANDS; i++)
        lc_str_printf(out, "  %-9s %s\n", commands[i].name,
                      commands[i].summary);
    lc_str_puts(out, main_help_tail);
}

static int print_help(cli *c, const cli_cmd *cmd)
{
    lc_str out = LC_STR_INIT;
    lc_json j;
    size_t i;
    int rc;

    if (c->json) {
        lc_json_init(&j, &out);
        lc_json_begin_object(&j);
        lc_json_key(&j, "schema");
        lc_json_int(&j, LC_JSON_SCHEMA);
        lc_json_key(&j, "commands");
        lc_json_begin_array(&j);
        for (i = 0; i < NCOMMANDS; i++)
            if (!cmd || cmd == &commands[i])
                help_json_command(&j, &commands[i]);
        lc_json_end_array(&j);
        lc_json_end_object(&j);
        rc = cli_emit_json(c, NULL, &j, &out);
    } else {
        help_text(&out, cmd);
        rc = out.failed ? cli_error(c, NULL, "out of memory")
                        : cli_write(out.data, out.len);
    }
    lc_str_free(&out);
    return rc;
}

static int cmd_help(cli *c)
{
    const cli_cmd *target = NULL;

    if (c->nargs > 0) {
        target = find_command(c->args[0]);
        if (!target)
            return cli_usage(c, "unknown command '%s'", c->args[0]);
    }
    return print_help(c, target);
}

static int cmd_version(cli *c)
{
    lc_str out = LC_STR_INIT;
    lc_json j;
    int rc;

    if (c->json) {
        lc_json_init(&j, &out);
        lc_json_begin_object(&j);
        lc_json_key(&j, "schema");
        lc_json_int(&j, LC_JSON_SCHEMA);
        lc_json_key(&j, "version");
        lc_json_string(&j, lc_version());
        lc_json_end_object(&j);
        rc = cli_emit_json(c, NULL, &j, &out);
    } else {
        lc_str_printf(&out, "lc %s\n", lc_version());
        rc = out.failed ? cli_error(c, NULL, "out of memory")
                        : cli_write(out.data, out.len);
    }
    lc_str_free(&out);
    return rc;
}

/* ------------------------------------------------------------------ */
/* Argument parsing                                                    */
/* ------------------------------------------------------------------ */

enum { O_JSON, O_HELP, O_COLOR, O_NO_PAGER, O_DIFFICULTY, O_TAG };

static const struct opt_def {
    const char *name;
    unsigned need;  /* CLI_OPT_* the command must take; 0 = every command */
    int has_value;
    int id;
} opt_defs[] = {
    {"--json", 0, 0, O_JSON},
    {"--help", 0, 0, O_HELP},
    {"-h", 0, 0, O_HELP},
    {"--color", CLI_OPT_COLOR, 1, O_COLOR},
    {"--no-pager", CLI_OPT_NO_PAGER, 0, O_NO_PAGER},
    {"--difficulty", CLI_OPT_DIFFICULTY, 1, O_DIFFICULTY},
    {"--tag", CLI_OPT_TAG, 1, O_TAG},
};

static void parse_error(lc_str *err, const char *fmt, ...) LC_PRINTF(2, 3);

/* Keeps the first error only. */
static void parse_error(lc_str *err, const char *fmt, ...)
{
    va_list ap;

    if (err->len > 0 || err->failed)
        return;
    va_start(ap, fmt);
    lc_str_vprintf(err, fmt, ap);
    va_end(ap);
}

static const struct opt_def *find_option(const cli_cmd *cmd, const char *name,
                                         size_t len)
{
    size_t i;

    for (i = 0; i < sizeof opt_defs / sizeof opt_defs[0]; i++) {
        const struct opt_def *o = &opt_defs[i];
        if (strlen(o->name) == len && memcmp(o->name, name, len) == 0 &&
            (o->need == 0 || (cmd->opts & o->need)))
            return o;
    }
    return NULL;
}

/* "easy,medium" -> bits in c->difficulties. */
static void parse_difficulties(cli *c, const char *list, lc_str *err)
{
    const char *p = list, *e;

    for (;;) {
        char buf[16];
        const char *a, *b;
        lc_difficulty d;

        e = strchr(p, ',');
        if (!e)
            e = p + strlen(p);
        for (a = p; a < e && *a == ' '; a++)
            ;
        for (b = e; b > a && b[-1] == ' '; b--)
            ;
        if (a == b) {
            parse_error(err, "empty entry in --difficulty list '%s'", list);
            return;
        }
        d = LC_DIFF_UNKNOWN;
        if ((size_t)(b - a) < sizeof buf) {
            memcpy(buf, a, (size_t)(b - a));
            buf[b - a] = '\0';
            d = lc_difficulty_from_name(buf);
        }
        if (d == LC_DIFF_UNKNOWN) {
            parse_error(err, "unknown difficulty '%.*s' (expected easy, "
                        "medium or hard)", (int)(b - a), a);
            return;
        }
        c->difficulties |= 1u << d;
        if (!*e)
            return;
        p = e + 1;
    }
}

static void apply_option(cli *c, const struct opt_def *o, const char *value,
                         lc_str *err)
{
    switch (o->id) {
    case O_JSON:
        c->json = 1;
        break;
    case O_HELP:
        c->help = 1;
        break;
    case O_NO_PAGER:
        c->no_pager = 1;
        break;
    case O_COLOR:
        if (strcmp(value, "auto") == 0)
            c->color = CLI_COLOR_AUTO;
        else if (strcmp(value, "always") == 0)
            c->color = CLI_COLOR_ALWAYS;
        else if (strcmp(value, "never") == 0)
            c->color = CLI_COLOR_NEVER;
        else
            parse_error(err, "invalid --color value '%s' (expected auto, "
                        "always or never)", value);
        break;
    case O_DIFFICULTY:
        parse_difficulties(c, value, err);
        break;
    case O_TAG:
        c->tags[c->ntags++] = (char *)value;
        break;
    }
}

/*
 * Parses the arguments after the command name. Options may come before or
 * after positional arguments, take values as "--opt=value" or "--opt value",
 * and "--" ends them. Every argument is read even after an error, so that
 * --json and --help count wherever they are; the first error is kept.
 */
static void parse_args(cli *c, int argc, char **argv, lc_str *err)
{
    int i, opts_done = 0;

    for (i = 0; i < argc; i++) {
        const char *a = argv[i], *value = NULL, *eq;
        const struct opt_def *o;
        size_t len;

        if (!opts_done && strcmp(a, "--") == 0) {
            opts_done = 1;
            continue;
        }
        if (opts_done || a[0] != '-' || a[1] == '\0') {
            c->args[c->nargs++] = argv[i];
            continue;
        }
        len = strlen(a);
        if (a[1] == '-' && (eq = strchr(a, '=')) != NULL) {
            len = (size_t)(eq - a);
            value = eq + 1;
        }
        o = find_option(c->cmd, a, len);
        if (!o) {
            parse_error(err, "unknown option '%.*s'", (int)len, a);
            continue;
        }
        if (!o->has_value) {
            if (value)
                parse_error(err, "option '%s' does not take a value", o->name);
            else
                apply_option(c, o, NULL, err);
            continue;
        }
        if (!value) {
            if (i + 1 >= argc) {
                parse_error(err, "option '%s' needs a value", o->name);
                continue;
            }
            value = argv[++i];
        }
        if (!*value)
            parse_error(err, "option '%s' needs a value", o->name);
        else
            apply_option(c, o, value, err);
    }
    if (c->nargs < c->cmd->min_args)
        parse_error(err, "%s", c->cmd->missing);
    else if (c->nargs > c->cmd->max_args)
        parse_error(err, "unexpected argument '%s'",
                    c->args[c->cmd->max_args]);
}

/* 1 if "--json" appears among the options in argv[0..argc). */
static int wants_json(int argc, char **argv)
{
    int i;

    for (i = 0; i < argc && strcmp(argv[i], "--") != 0; i++)
        if (strcmp(argv[i], "--json") == 0)
            return 1;
    return 0;
}

int main(int argc, char **argv)
{
    cli c;
    lc_str err = LC_STR_INIT;
    const char *name;
    int rc;

    /* A closed pipe shows up as EPIPE from write(), handled in pager.c. */
    signal(SIGPIPE, SIG_IGN);
    memset(&c, 0, sizeof c);
    c.color = CLI_COLOR_AUTO;

    if (argc < 2) {
        lc_str out = LC_STR_INIT;

        help_text(&out, NULL);
        fputs(out.failed ? "usage: lc <command> [options] [arguments]\n"
                         : out.data, stderr);
        lc_str_free(&out);
        return CLI_EXIT_ERROR;
    }
    name = argv[1];
    if (strcmp(name, "--help") == 0 || strcmp(name, "-h") == 0)
        name = "help";
    else if (strcmp(name, "--version") == 0)
        name = "version";
    c.cmd = find_command(name);
    if (!c.cmd) {
        size_t i, len = strcspn(name, "=");

        c.json = wants_json(argc - 1, argv + 1);
        if (name[0] != '-' || name[1] == '\0')
            return cli_usage(&c, "unknown command '%s'", name);
        for (i = 0; i < sizeof opt_defs / sizeof opt_defs[0]; i++)
            if (strlen(opt_defs[i].name) == len &&
                memcmp(opt_defs[i].name, name, len) == 0)
                return cli_usage(&c, "option '%.*s' must come after a "
                                 "command", (int)len, name);
        return cli_usage(&c, "unknown option '%.*s'", (int)len, name);
    }

    /* Room for every argument, as a positional or a --tag value. */
    c.args = malloc((size_t)argc * sizeof *c.args);
    c.tags = malloc((size_t)argc * sizeof *c.tags);
    if (!c.args || !c.tags) {
        free(c.args);
        free(c.tags);
        c.json = wants_json(argc - 2, argv + 2);
        return cli_error(&c, NULL, "out of memory");
    }
    parse_args(&c, argc - 2, argv + 2, &err);
    if (c.help)
        rc = print_help(&c, c.cmd);
    else if (err.failed)
        rc = cli_error(&c, NULL, "out of memory");
    else if (err.len > 0)
        rc = cli_usage(&c, "%s", err.data);
    else
        rc = c.cmd->run(&c);
    lc_str_free(&err);
    free(c.args);
    free(c.tags);
    return rc;
}
