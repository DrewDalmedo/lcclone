# JSON output

Every `lc` command takes `--json`. With it, `lc` prints one JSON document on
stdout: a single line of compact JSON followed by a newline. Editor plugins
and GUIs run `lc` as a subprocess and read that line, so nothing ever has to
scrape the text output. This document describes milestone M0: `list`,
`show`, `help` and `version`.

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

A document is in JSON mode when `--json` appears among the options, even if
the error is about another argument (`lc list --bogus --json`). A `--json`
after `--` is an ordinary argument.

Exit statuses: 0 on success; 3 for every error. Statuses 1 and 2 are kept for
judging verdicts (milestone M3).

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
| `statement`       | string          | the statement's markdown, exactly as written in `problem.md` |
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
| `code`      | `lang` (the fence's info string, `""` if none), `text` (the contents verbatim, ending in `"\n"` unless empty) |
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
