# JSON output

Every `lc` command takes `--json`. With it, `lc` prints one JSON document on
stdout: a single line of compact JSON followed by a newline. Editor plugins
and GUIs run `lc` as a subprocess and read that line, so nothing ever has to
scrape the text output. `lc test` and `lc submit` are the exception: they
print one such line per event, as it happens ([JSON Lines](#lc-test---json-and-lc-submit---json)),
so a front-end can show progress.

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
130 when ^C stopped the run.

## `lc list --json`

```json
{"schema":1,"problems":[{"id":1,"slug":"two-sum","title":"Two Sum","difficulty":"easy","tags":["array","hash-table"]}],"warnings":[]}
```

`problems` holds the problems that pass the `--difficulty` and `--tag`
filters, in ID order. It is an empty array when nothing matches.

| Member       | Type             | Meaning |
|--------------|------------------|---------|
| `id`         | integer          | the LeetCode number |
| `slug`       | string           | e.g. `"two-sum"` |
| `title`      | string           | e.g. `"Two Sum"` |
| `difficulty` | string           | `"easy"`, `"medium"` or `"hard"` |
| `tags`       | array of strings | in the order of the pack's `tags:` line; may be empty |

## `lc show <problem> --json`

```json
{"schema":1,"problem":{"id":1,"slug":"two-sum","title":"Two Sum","difficulty":"easy","tags":["array","hash-table"],"signature":"int[] twoSum(int[] nums, int target)","time_limit_ms":1000,"memory_limit_mb":256,"dir":"/home/me/lcclone/problems/0001-two-sum","statement":"You are given ...\n","statement_line":10,"blocks":[...],"examples":[...]},"warnings":[]}
```

| Member            | Type            | Meaning |
|-------------------|-----------------|---------|
| `id`, `slug`, `title`, `difficulty`, `tags` | | as in `lc list` |
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
