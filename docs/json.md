# JSON output

Every `lc` command takes `--json`. With it, `lc` prints one JSON document on
stdout: a single line of compact JSON followed by a newline. Editor plugins
and GUIs run `lc` as a subprocess and read that line, so nothing ever has to
scrape the text output. `lc test` and `lc submit` are the exception: they
print one such line per event, as it happens ([JSON Lines](#lc-test---json-and-lc-submit---json)),
so a front-end can show progress; so does `lc check`.

## General rules

- **Schema version.** Every document is an object whose `"schema"` member is
  `1` (`LC_JSON_SCHEMA` in `include/lc.h`). Members may be added within a
  schema version; removing or changing one bumps the version. Ignore members
  you don't know.
- **Encoding.** Strings are UTF-8. Bytes that are not valid UTF-8 become
  U+FFFD, and control characters are escaped.
- **Paths** are absolute, because the core resolves the problem directory
  with `realpath`.
- **Warnings.** Non-fatal problems, such as a problem pack that failed to load
  and was skipped, are listed in `"warnings"`: an array of strings, usually
  `<file>:<line>: <message>`. They are also printed on stderr as
  `lc: warning: <message>`, in JSON mode as in text mode.
- **stderr is for people.** In JSON mode stdout carries only the document;
  stderr carries the same messages as in text mode.
- **Colour and pager.** `--json` output is never coloured or paged, whatever
  `--color` says.

## Errors

When a command fails (a usage error, an unknown problem, a problem directory
that can't be used, running out of memory), stdout holds an error document
and `lc` exits with status 3:

```json
{"schema":1,"error":{"message":"no problem matches '999'"},"warnings":[]}
```

| Member          | Type   | Meaning |
|-----------------|--------|---------|
| `error.message` | string | the same text that stderr shows after `lc: ` |
| `warnings`      | array  | warnings gathered before the failure, such as skipped packs (they can explain why a problem is missing) |

If there isn't even the memory to build the error document, `lc` prints
this fixed one, without the warnings, and stderr has the line
`lc: out of memory`:

```json
{"schema":1,"error":{"message":"out of memory"},"warnings":[]}
```

A document is in JSON mode when `--json` appears among the options, even if
the error is about another argument (`lc list --bogus --json`). A `--json`
after `--` is an ordinary argument.

Exit statuses: 0 on success; 3 for every error. `lc test` and `lc submit`
also exit with 1 for a verdict other than Accepted, 2 for Compile Error and
130 when ^C stopped the run; `lc check` with 1 when a pack fails, and 130
when ^C stopped it.

## `lc list --json`

```json
{"schema":1,"problems":[{"id":1,"slug":"two-sum","title":"Two Sum","difficulty":"easy","tags":["array","hash-table"],"status":"solved"}],"warnings":[]}
```

`problems` holds the problems that pass the `--difficulty`, `--tag` and
`--status` filters, in ID order. It is an empty array when nothing
matches.

| Member       | Type             | Meaning |
|--------------|------------------|---------|
| `id`         | integer          | the LeetCode number |
| `slug`       | string           | e.g. `"two-sum"` |
| `title`      | string           | e.g. `"Two Sum"` |
| `difficulty` | string           | `"easy"`, `"medium"` or `"hard"` |
| `tags`       | array of strings | in the order of the pack's `tags:` line; may be empty |
| `status`     | string           | your progress: `"todo"`, `"attempted"` or `"solved"` ([workspace.md](workspace.md#progress-and-history)) |

## `lc random --json`

```json
{"schema":1,"problem":{"id":121,"slug":"best-time-to-buy-and-sell-stock","title":"Best Time to Buy and Sell Stock","difficulty":"easy","tags":["array","dynamic-programming"],"status":"todo"},"candidates":7,"warnings":[]}
```

`problem` is the problem picked, with the members of a `lc list` entry,
and `candidates` how many passed the filters. When none does, the output
is an error document.

## `lc stats --json`

```json
{"schema":1,"total":11,"solved":3,"attempted":1,"difficulties":[{"difficulty":"easy","total":10,"solved":3,"attempted":1},...],"tags":[{"tag":"array","total":5,"solved":2,"attempted":0},...],"warnings":[]}
```

| Member         | Type    | Meaning |
|----------------|---------|---------|
| `total`        | integer | the problems loaded |
| `solved`, `attempted` | integer | of them, how many you solved, and attempted without solving |
| `difficulties` | array   | `easy`, `medium` and `hard`, in that order: `difficulty`, `total`, `solved`, `attempted` |
| `tags`         | array   | every tag of the loaded problems, by name: `tag`, `total`, `solved`, `attempted` |

## `lc stats --manifest --json`

```json
{"schema":1,"manifest":"/home/me/lcclone/problems/MANIFEST.tsv","total":11,"premium":0,"skipped":0,"todo":0,"draft":1,"verified":10,"groups":[{"group":"plain","total":7,"todo":0,"draft":0,"verified":7},...],"skips":[],"no_pack":[],"todo_with_pack":[],"unlisted":[],"warnings":[]}
```

| Member      | Type    | Meaning |
|-------------|---------|---------|
| `manifest`  | string  | the file |
| `total`     | integer | its rows |
| `premium`   | integer | of them, premium-only problems |
| `skipped`   | integer | rows out of scope (`skip:REASON`) |
| `todo`, `draft`, `verified` | integer | the rows in scope, by status |
| `groups`    | array   | the rows in scope by group, in the order first seen: `group`, `total`, `todo`, `draft`, `verified` |
| `skips`     | array   | the rows out of scope by reason: `reason` and the same counts |
| `no_pack`   | array of integers | IDs marked draft or verified that have no pack |
| `todo_with_pack` | array of integers | IDs marked todo that have a pack |
| `unlisted`  | array of integers | IDs of packs that have no row |

## `lc show <problem> --json`

```json
{"schema":1,"problem":{"id":1,"slug":"two-sum","title":"Two Sum","difficulty":"easy","tags":["array","hash-table"],"signature":"int[] twoSum(int[] nums, int target)","time_limit_ms":1000,"memory_limit_mb":256,"dir":"/home/me/lcclone/problems/0001-two-sum","statement":"You are given ...\n","statement_line":10,"blocks":[...],"examples":[...]},"warnings":[]}
```

| Member            | Type            | Meaning |
|-------------------|-----------------|---------|
| `id`, `slug`, `title`, `difficulty`, `tags`, `status` | | as in `lc list` |
| `progress`        | object          | `lang` (`"c"` or `"cpp"`: of the last accepted submission, else of the last one), `best_ms` (the slowest case of the best accepted submission) and `last_submitted` (seconds since 1970); each null when there is none |
| `signature`       | string or null  | the raw `signature:` header value |
| `time_limit_ms`   | integer         | default 1000 |
| `memory_limit_mb` | integer         | default 256 |
| `dir`             | string          | absolute path of the pack directory |
| `statement`       | string          | the statement's markdown as written in `problem.md`, except that a `\r` before each newline is dropped (see `docs/problem-format.md`) |
| `statement_line`  | integer         | the line of `problem.md` where the statement starts |
| `blocks`          | array           | the statement parsed into blocks (below) |
| `examples`        | array           | the cases of `tests.txt` marked `@example` (below) |

If `tests.txt` can't be read or parsed, the statement is still shown:
`examples` is empty and the error is added to `warnings` (and printed on
stderr as a warning). A pack without `tests.txt` simply has no examples.

### Blocks

The statement markdown (the subset in `docs/problem-format.md`) as a flat
list of blocks, in document order. Every block has:

| Member | Type    | Meaning |
|--------|---------|---------|
| `type` | string  | `"paragraph"`, `"heading"`, `"code"`, `"list_item"`, `"quote"`, `"rule"` or `"examples"` |
| `line` | integer | 1-based line within the statement; its line in `problem.md` is `statement_line + line - 1` |

and, depending on the type:

| Type        | Members |
|-------------|---------|
| `paragraph` | `spans` |
| `heading`   | `level` (1 to 6), `spans` |
| `code`      | `lang` (the first word of the fence's info string, `""` if none), `text` (the contents verbatim, ending in `"\n"` unless empty) |
| `list_item` | `ordered` (boolean), `number` (integer for a numbered item, null for a bulleted one), `depth` (0 for top level, 1 for an item nested once, ...), `spans` |
| `quote`     | `spans`: one paragraph of a `>` block quote (an empty `>` line starts a new quote block) |
| `rule`      | nothing more: a thematic break |
| `examples`  | nothing more: the `@examples` line, which marks where to show the examples. A statement without one gets its examples after the last block. `lc show` uses the first one if there are several. |

A list is a run of `list_item` blocks; there is no enclosing list block.

### Spans

`spans` is the inline text of a block, split wherever its formatting
changes. Each span has `text` (escapes already removed, line breaks already
turned into spaces) and only those flags that are set:

| Member   | Type    | Meaning |
|----------|---------|---------|
| `strong` | `true`  | `**strong**` |
| `em`     | `true`  | `*emphasis*` |
| `code`   | `true`  | a code span; never combined with `strong` or `em` |
| `link`   | string  | the span is (part of) a link to this URL. A link whose text has formatting is several spans with the same `link` |
| `break`  | `true`  | a hard line break; `text` is `""` |

For example `**indices** of [the notes](https://example.com)` gives

```json
[{"text":"indices","strong":true},{"text":" of "},{"text":"the notes","link":"https://example.com"}]
```

### Examples

```json
{"inputs":[{"name":"nums","value":"[4,1,9]"},{"name":"target","value":"13"}],"output":"[0,2]","explanation":"4 + 9 = 13."}
```

| Member        | Type           | Meaning |
|---------------|----------------|---------|
| `inputs`      | array          | one entry per input line of the case, in order |
| `inputs[].name`  | string or null | the argument's name, from the signature's parameters followed by the `hidden:` ones. Null for every input when the number of input lines differs from the number of names (for example, a pack without a signature) |
| `inputs[].value` | string      | the literal exactly as written (spaces at the ends trimmed). M0 does not parse literals, so a string literal keeps its quotes: `"\"abc\""` |
| `output`      | string         | the expected output literal, as written after `=>` |
| `explanation` | string or null | the text after `@example`, or null |

## `lc start <problem> --json`

Sets up the problem's directory (never opening an editor) and describes
it:

```json
{"schema":1,"problem":{"id":1,"slug":"two-sum","title":"Two Sum"},"lang":"cpp","dir":"/home/me/lc/0001-two-sum","solution":"/home/me/lc/0001-two-sum/solution.cpp","readme":"/home/me/lc/0001-two-sum/README.md","cases":"/home/me/lc/0001-two-sum/cases.txt","created_solution":true,"created_cases":true,"warnings":[]}
```

| Member             | Type    | Meaning |
|--------------------|---------|---------|
| `problem`          | object  | `id`, `slug` and `title`, as in `lc list` |
| `lang`             | string  | `"c"` or `"cpp"` |
| `dir`              | string  | the problem's directory in the workspace |
| `solution`         | string  | the solution file |
| `readme`, `cases`  | string  | `README.md` and `cases.txt` |
| `created_solution` | boolean | the solution file was just created from the stub (false: it was there already and was left alone) |
| `created_cases`    | boolean | the same for `cases.txt` |

## `lc test --json` and `lc submit --json`

These print **JSON Lines**: one object per line, each written as soon as
the event happens. Every line has `"schema":1` and an `"event"`, and the
last line is either a `done` event or an error document (as above, for
errors before or during the run). For example:

```
{"schema":1,"event":"build_start","reference":false,"lang":"cpp","std":"c++11","source":"/home/me/lc/0001-two-sum/solution.cpp","compiler":"c++","sanitize":false}
{"schema":1,"event":"build_done","reference":false,"lang":"cpp","std":"c++11","source":"...","compiler":"c++","sanitize":false,"ok":true,"cached":false,"time_ms":412,"output":"","diagnostics":[]}
{"schema":1,"event":"case_start","index":0,"total":3,"file":"/home/me/lcclone/problems/0001-two-sum/tests.txt","line":5}
{"schema":1,"event":"case_done","total":3,"index":0,"file":"...","line":5,"example":true,"custom":false,"verdict":"accepted","message":null,"inputs":[{"name":"nums","value":"[5,2,7,9]"},{"name":"target","value":"11"}],"expected":"[1,3]","output":"[1,3]","stdout":"","stdout_truncated":false,"stderr":"","stderr_truncated":false,"time_ms":2,"wall_ms":3,"memory_kb":1360,"exit_code":0,"signal":null}
{"schema":1,"event":"done","verdict":"accepted","passed":3,"total":3,"cancelled":false,"max_time_ms":2,"max_memory_kb":1360,"warnings":[]}
```

### `build_start`, `build_done`

A source file is about to be built, or was. There is one pair for your
solution and, when your cases need expected results worked out, one for
the problem's reference solution, after it.

| Member        | Type    | Meaning |
|---------------|---------|---------|
| `reference`   | boolean | this is the reference solution, not yours |
| `role`        | string  | what is built: `"solution"` (yours) or `"reference"`; for `lc check` also `"brute"`, `"stub"` and `"generator"` |
| `lang`        | string  | `"c"` or `"cpp"` |
| `std`         | string  | the standard, e.g. `"c++17"` |
| `source`      | string  | the source file |
| `compiler`    | string  | the compiler command, e.g. `"c++"` |
| `sanitize`    | boolean | built with the sanitizers |

`build_done` adds:

| Member        | Type    | Meaning |
|---------------|---------|---------|
| `ok`          | boolean | it compiled |
| `cached`      | boolean | an earlier build of exactly the same input was reused |
| `time_ms`     | integer | how long compiling took; 0 when cached |
| `output`      | string  | what the compiler printed (empty when cached) |
| `diagnostics` | array   | the compiler's messages, below |

Each diagnostic has `file` (as the compiler printed it: your solution's
absolute path, `"lc_harness"` for lc's harness, or a tool such as `"ld"`),
`line` and `column` (0 when not given), `severity` (`"error"`, `"warning"`
or `"note"`), `message`, and `in_solution` (the file is your solution).

### `case_start`, `case_done`

| Member         | Type            | Meaning |
|----------------|-----------------|---------|
| `index`        | integer         | the case's position in the run, from 0 |
| `total`        | integer         | how many cases the run has |
| `file`, `line` | string, integer | where the case is: the pack's `tests.txt`, or your cases file |

`case_done` adds:

| Member      | Type            | Meaning |
|-------------|-----------------|---------|
| `example`   | boolean         | an `@example` case of the pack |
| `custom`    | boolean         | one of your cases |
| `verdict`   | string          | `"accepted"`, `"wrong_answer"`, `"runtime_error"`, `"time_limit_exceeded"`, `"memory_limit_exceeded"` or `"output_limit_exceeded"` |
| `message`   | string or null  | why, for verdicts other than Accepted and Wrong Answer, e.g. `"killed by SIGSEGV"` |
| `inputs`    | array           | `name` (or null) and `value`, as for examples in `lc show` |
| `expected`  | string          | the expected result, as a literal |
| `output`    | string or null  | the solution's result, as a literal; null when there was none that could be read |
| `stdout`    | string          | the first 64 KiB of what it printed on stdout |
| `stderr`    | string          | the last 64 KiB of its stderr |
| `stdout_truncated`, `stderr_truncated` | boolean | they were cut |
| `time_ms`   | integer         | CPU time |
| `wall_ms`   | integer         | elapsed time |
| `memory_kb` | integer         | peak resident memory |
| `exit_code` | integer or null | its exit status; null when a signal ended it |
| `signal`    | string or null  | the signal that ended it, e.g. `"SIGSEGV"` (a number if it has no name) |

### `done`

| Member          | Type    | Meaning |
|-----------------|---------|---------|
| `verdict`       | string  | the run's verdict: that of the first case that failed, `"accepted"`, `"compile_error"`, or `"none"` when it was cancelled first |
| `passed`        | integer | cases accepted |
| `total`         | integer | cases in the run |
| `cancelled`     | boolean | ^C stopped it |
| `max_time_ms`   | integer | the slowest case's CPU time |
| `max_memory_kb` | integer | the largest peak memory of any case |
| `warnings`      | array   | as in other documents |

`lc submit` records the submission ([workspace.md](workspace.md#progress-and-history)),
and its `done` event adds:

| Member        | Type           | Meaning |
|---------------|----------------|---------|
| `status`      | string or null | the problem's status now: `"attempted"` or `"solved"`; null when nothing was recorded (a cancelled run, or progress.tsv couldn't be written, which a warning explains) |
| `first_solve` | boolean        | this submission solved the problem for the first time |
| `saved`       | string or null | the copy of the solution in the history |

## `lc check --json`

JSON Lines, like `lc test --json`: every line has `"schema":1`, an
`"event"` and, except `done`, the `"pack"` it is about (its directory
name, such as `"0001-two-sum"`, or null for `MANIFEST.tsv` as a whole).

```
{"schema":1,"event":"pack_start","pack":"0121-best-time-to-buy-and-sell-stock","id":121,"dir":"/home/me/lcclone/problems/0121-best-time-to-buy-and-sell-stock"}
{"schema":1,"event":"build_start","pack":"0121-best-time-to-buy-and-sell-stock","reference":false,"role":"stub","lang":"c",...}
{"schema":1,"event":"finding","pack":"0121-best-time-to-buy-and-sell-stock","level":"error","file":"/home/me/.../ref.c","line":null,"message":"ref.c and brute.c disagree on the random case for seed 80 (small)","detail":"Input:    prices = [7,16,13]\nref.c:    10\nbrute.c:  12\n"}
{"schema":1,"event":"pack_done","pack":"0121-best-time-to-buy-and-sell-stock","status":"failed","errors":1,"warnings":0,"skipped":null,"cases":14,"examples":3,"generated":3,"ref":"ref.c","brute":"brute.c","gen":"gen.c","stubs":2,"ref_passed":14,"ref_max_ms":2,"cross":79,"regen":null}
{"schema":1,"event":"done","packs":1,"ok":0,"skipped":0,"failed":1,"manifest_ok":true,"cancelled":false,"warnings":[]}
```

- **`pack_start`**: `id` and `dir`. A pack that fails to load has no
  `pack_start`, only its findings and a `pack_done` with just `status`,
  `errors` and `warnings`.
- **`build_start`, `build_done`**: as for `lc test`, with `role`.
- **`finding`**: `level` (`"error"`, `"warning"` or `"note"`), `file`
  (absolute, or null for the pack as a whole), `line` (or null),
  `message` (one line) and `detail` (lines such as a case's input and
  outputs, or a compiler's output; or null).
- **`pack_done`**: `status` is `"ok"`, `"failed"` (errors were found) or
  `"skipped"` (its solutions were not built or run: `skipped` says why).
  The counts: `cases`, `examples` and `generated` (`@gen`) in
  tests.txt, `stubs` built, `ref_passed` cases and `ref_max_ms`, and
  `cross` random cases `ref` and `brute` agreed on. `ref`, `brute` and
  `gen` name the pack's files, or are null. With `--regen`, `regen` is
  `written`, `changed` (expected results written) and `generated`
  (cases made).
- **`done`**: the counts of packs, `manifest_ok`, and whether ^C
  `cancelled` the run.

## `lc help --json`

```json
{"schema":1,"commands":[{"name":"list","summary":"List problems, filtered by difficulty and tags","usage":"lc list [--difficulty LIST] [--tag TAG]... [--json] [--color=WHEN]"}]}
```

`commands` lists every command, or only the one asked about with
`lc help <command> --json` (or `lc <command> --help --json`).

| Member    | Type   | Meaning |
|-----------|--------|---------|
| `name`    | string | the command name |
| `summary` | string | one line describing it |
| `usage`   | string | its synopsis |

## `lc version --json`

```json
{"schema":1,"version":"0.1.0-dev"}
```
