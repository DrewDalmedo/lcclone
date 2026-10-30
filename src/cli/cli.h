/*
 * cli.h: declarations shared by the files of the lc command-line front-end.
 *
 * The CLI uses only the public API in include/lc.h. It builds every piece
 * of output in an lc_str first and writes it in one go (pager.c), so a
 * failed allocation never leaves half a table on the screen.
 */
#ifndef LC_CLI_H
#define LC_CLI_H

#include <stddef.h>

#include "lc.h"

/* Exit statuses (docs/PLAN.md section 3). */
#define CLI_EXIT_OK 0           /* success; Accepted */
#define CLI_EXIT_VERDICT 1      /* any verdict other than Accepted */
#define CLI_EXIT_COMPILE 2      /* Compile Error */
#define CLI_EXIT_ERROR 3        /* usage and other errors */
#define CLI_EXIT_CANCELLED 130  /* stopped by ^C */

/* --color=WHEN */
enum { CLI_COLOR_AUTO, CLI_COLOR_ALWAYS, CLI_COLOR_NEVER };

/* Options a command takes besides --json and --help (cli_cmd.opts). */
enum {
    CLI_OPT_COLOR = 1 << 0,
    CLI_OPT_NO_PAGER = 1 << 1,
    CLI_OPT_DIFFICULTY = 1 << 2,
    CLI_OPT_TAG = 1 << 3,
    CLI_OPT_LANG = 1 << 4,
    CLI_OPT_STD = 1 << 5,
    CLI_OPT_SANITIZE = 1 << 6,
    CLI_OPT_CASE = 1 << 7,
    CLI_OPT_NO_EDIT = 1 << 8
};

typedef struct cli cli;

typedef struct cli_cmd {
    const char *name;
    const char *summary;    /* one line, for 'lc help' */
    const char *usage;      /* "lc list [--difficulty LIST] ..." */
    const char *help;       /* description and options, for 'lc help NAME' */
    unsigned opts;          /* CLI_OPT_* */
    size_t min_args;        /* positional arguments */
    size_t max_args;
    const char *missing;    /* message when a positional argument is missing */
    int (*run)(cli *c);
} cli_cmd;

/* A parsed command line. Strings point into argv. */
struct cli {
    const cli_cmd *cmd;
    int json;
    int help;
    int color;              /* CLI_COLOR_* */
    int no_pager;
    unsigned difficulties;  /* bit (1 << lc_difficulty) per wanted one; 0 = all */
    char **args;            /* positional arguments */
    size_t nargs;
    const char **tags;      /* --tag values; a problem must have all of them */
    size_t ntags;
    const char *lang;       /* --lang */
    const char *std;        /* --std */
    int sanitize;           /* --sanitize */
    const char *case_file;  /* --case */
    int no_edit;            /* --no-edit */
};

/* ------------------------------------------------------------------ */
/* main.c                                                              */
/* ------------------------------------------------------------------ */

/*
 * Reports an error and returns CLI_EXIT_ERROR: "lc: <message>" on stderr
 * and, with --json, an error object on stdout. `ctx` (may be NULL) supplies
 * the warnings for the JSON object.
 */
int cli_error(cli *c, lc_ctx *ctx, const char *fmt, ...) LC_PRINTF(3, 4);

/* Like cli_error, for usage errors: adds "Run 'lc help <command>'." */
int cli_usage(cli *c, const char *fmt, ...) LC_PRINTF(2, 3);

/* Prints "lc: warning: <message>" on stderr. */
void cli_warn(const char *msg);

/*
 * Opens a context and loads the problem packs, printing the loader's
 * warnings. Returns CLI_EXIT_OK and sets *out, or reports the error.
 */
int cli_load(cli *c, lc_ctx **out);

/* Writes "warnings": [...] into an open JSON object: the context's
 * warnings, then `extra` unless it is NULL. */
void cli_json_warnings(lc_json *j, const lc_ctx *ctx, const char *extra);

/*
 * Finishes a JSON document built in `out` with lc_json, adds the newline
 * and writes it to stdout. Returns the exit status.
 */
int cli_emit_json(cli *c, lc_ctx *ctx, lc_json *j, lc_str *out);

/* ------------------------------------------------------------------ */
/* cmd_list.c, cmd_show.c                                              */
/* ------------------------------------------------------------------ */

int cmd_list(cli *c);
int cmd_show(cli *c);

/* ------------------------------------------------------------------ */
/* cmd_start.c, cmd_test.c                                             */
/* ------------------------------------------------------------------ */

int cmd_start(cli *c);
int cmd_test(cli *c);
int cmd_submit(cli *c);

/* The language --lang names, or NULL after reporting a usage error
 * (*rc is set). NULL with *rc = CLI_EXIT_OK when there is no --lang. */
const lc_lang *cli_lang_option(cli *c, int *rc);

/*
 * A path as the user should see it: relative to the current directory
 * when it is inside it, else as it is. Appended to `out`.
 */
void cli_show_path(lc_str *out, const char *path);

/* ------------------------------------------------------------------ */
/* ansi.c: colour, terminal width and word wrapping                    */
/* ------------------------------------------------------------------ */

/* Text styles; any combination. */
enum {
    CLI_ST_BOLD = 1 << 0,
    CLI_ST_ITALIC = 1 << 1,
    CLI_ST_UNDERLINE = 1 << 2,
    CLI_ST_CODE = 1 << 3,   /* code spans and code blocks */
    CLI_ST_GREEN = 1 << 4,
    CLI_ST_YELLOW = 1 << 5,
    CLI_ST_RED = 1 << 6
};

/* 1 if colour should be used for a --color mode (see 'lc help list'). */
int cli_color_enabled(int mode);

/* Width to wrap text at: $COLUMNS, else the terminal's, else 80; then
 * clamped to [20, 100]. */
size_t cli_term_width(void);

/* Display columns of UTF-8 text: code points, with escape sequences and
 * other control characters counted as zero. */
size_t cli_text_width(const char *s, size_t n);

/* Switches the output from style *cur to `want` (nothing without colour). */
void cli_set_style(lc_str *out, int color, unsigned *cur, unsigned want);

/* Appends text in a style and switches back to plain. */
void cli_put_styled(lc_str *out, int color, unsigned style, const char *s);

/* Appends n spaces. */
void cli_put_spaces(lc_str *out, size_t n);

/* "Easy", "Medium", "Hard" or "Unknown", and the colour for it. */
const char *cli_difficulty_label(lc_difficulty d);
unsigned cli_difficulty_style(lc_difficulty d);

/* A pending word: styled pieces of text with no blank between them. */
typedef struct cli_seg {
    unsigned style;
    size_t off;
    size_t len;
} cli_seg;

/*
 * Word wrapper. Feed it styled text; it breaks lines at blanks so that no
 * line is wider than `width` unless a single word is. A blank run between
 * two words on the same line is kept as written; at a line break it is
 * dropped. Every line starts with a prefix: `first` for the first line,
 * `rest` for the others (a hanging indent).
 *
 *     cli_wrap w;
 *     cli_wrap_begin(&w, out, color, width, "  - ", "    ");
 *     cli_wrap_text(&w, CLI_ST_BOLD, "Hello", 5);
 *     cli_wrap_end(&w);
 */
typedef struct cli_wrap {
    lc_str *out;
    int color;
    size_t width;
    const char *first;
    const char *rest;
    unsigned cur;       /* style in effect in `out` */
    size_t col;         /* display column on the current line */
    size_t line_start;  /* offset of the current line in `out` */
    int in_line;        /* the current line has its prefix */
    size_t nlines;      /* lines finished */
    size_t nwords;      /* words on the current line */
    lc_str word;        /* text of the pending word */
    cli_seg *segs;
    size_t nsegs;
    size_t capsegs;
    size_t gap;         /* blanks before the pending word */
    unsigned gap_style;
    int nomem;
} cli_wrap;

void cli_wrap_begin(cli_wrap *w, lc_str *out, int color, size_t width,
                    const char *first, const char *rest);
void cli_wrap_text(cli_wrap *w, unsigned style, const char *s, size_t n);
void cli_wrap_break(cli_wrap *w); /* hard line break */
/* Ends the last line (writing at least one line) and frees the wrapper's
 * memory. Returns LC_OK or LC_ERR_NOMEM. */
int cli_wrap_end(cli_wrap *w);

/* ------------------------------------------------------------------ */
/* pager.c: writing the output                                         */
/* ------------------------------------------------------------------ */

/*
 * Writes to stdout. A reader that went away (EPIPE) is a quiet stop, not
 * an error. Returns the exit status.
 */
int cli_write(const char *data, size_t len);

/*
 * Writes through a pager when `allow` is set and stdout is a terminal:
 * $PAGER (empty or "cat" means none), else less, else more. Falls back to
 * cli_write when there is no pager or it cannot be started.
 */
int cli_page(const char *data, size_t len, int allow);

#endif /* LC_CLI_H */
