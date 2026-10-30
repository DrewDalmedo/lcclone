# Problem pack format

A problem pack is a directory holding one problem. This document describes
the parts that milestone M0 reads: `problem.md`, the `@example` cases in
`tests.txt`, and the markdown subset used for statements. Later milestones add
`ref.c`, `gen.c`, `checker.c` and the other files listed in `docs/PLAN.md`
§5.5.

## Where packs live

`lc` uses the first of these that applies:

1. `lc_options.problems_dir`, when a front-end sets it;
2. `$LC_PROBLEMS`, if set (it must be a directory, or loading fails);
3. `$XDG_DATA_HOME/lcclone/problems`, default
   `~/.local/share/lcclone/problems`, if it exists;
4. `$(PREFIX)/share/lcclone/problems`, if it exists.

`make install` copies this repository's `problems/` to the last location.

## The pack directory

```
problems/0001-two-sum/
  problem.md     required: header and statement
  tests.txt      test cases; M0 shows the ones marked @example
```

**Name.** The directory is named `NNNN-slug`: the problem ID padded with zeros
to at least four digits (`printf("%04d-%s", id, slug)`), a dash and the slug.
Both must match `problem.md`.

**Which entries are packs.** Every entry of the problems directory whose name
starts with an ASCII digit and is a directory is loaded as a pack. Everything
else (files such as `MANIFEST.tsv`, directories starting with a letter or a
dot) is ignored.

**Bad packs are skipped, not fatal.** A pack that fails to load is left out of
the index and recorded as a warning that names the file and line, for example
`problems/0005-x/problem.md:3: unknown difficulty 'eazy'`. If two packs share
an ID or a slug, the one whose directory name sorts first is kept and the
other is skipped with a warning.

## problem.md

A header of `key: value` lines, a line containing only `---`, then the
statement in markdown.

```
id: 1
slug: two-sum
title: Two Sum
difficulty: easy
tags: array, hash-table
signature: int[] twoSum(int[] nums, int target)
compare: unordered 0
---
You have a list of integers `nums` and a goal value `target`. ...
```

### Header syntax

- One field per line: a key starting in the first column, a colon right
  after it, then the value. Spaces and tabs around the value are trimmed. The
  value may be empty.
- A key is a lowercase ASCII letter or `_`, followed by lowercase letters,
  digits or `_`.
- Blank lines (including lines of only spaces and tabs) and lines starting
  with `#` in the first column are ignored.
- A key may appear only once.
- The header ends at the first line that is exactly `---` (trailing spaces
  and tabs allowed). A file with no such line is an error.
- A UTF-8 byte order mark at the start of the file is skipped, and a `\r`
  before each newline is ignored, in the header and the statement alike.
- A file containing a NUL byte is an error.

### Fields read in M0

| Key               | Required | Meaning |
|-------------------|----------|---------|
| `id`              | yes      | the LeetCode number, a decimal integer from 1 to 99999 |
| `slug`            | yes      | lowercase letters and digits in groups joined by single dashes: `two-sum`, `3sum`, `2-keys-keyboard` |
| `title`           | yes      | non-empty |
| `difficulty`      | yes      | `easy`, `medium` or `hard` (lowercase) |
| `tags`            | no       | comma-separated; each tag uses lowercase letters, digits and dashes. Empty entries (`a,,b` or a trailing comma) are errors |
| `signature`       | no       | e.g. `int[] twoSum(int[] nums, int target)`; see below |
| `hidden`          | no       | extra inputs that only shape test data, e.g. `int pos`; see below |
| `time_limit_ms`   | no       | positive integer, default 1000 |
| `memory_limit_mb` | no       | positive integer, default 256 |

Any other key (`compare`, `class`, `api`, `check`, ...) is kept as-is for later
milestones; M0 does not interpret it. Every field, known or not, is available
through `lc_problem.fields` and `lc_problem_field()`.

### Argument names

To show an example as `nums = [1,2], target = 3`, the loader takes parameter
names from `signature` and `hidden`. M0 does not check types; that is M1.

- **From `signature`:** the text between the first `(` and the last `)` is
  split on commas that are not inside `<>`, `[]` or `()`. Each parameter is a
  type followed by a name, and the name is the identifier at its end
  (`[A-Za-z_][A-Za-z0-9_]*`). `()` gives no names.
- **From `hidden`:** a comma-separated list in the same form as parameters,
  e.g. `int pos` or `int skipA, int skipB`. Its names come after the
  signature's. Without a `signature` there are no names at all, but `hidden`
  is still checked.
- The pack fails to load if `signature` has no parentheses, or if a parameter
  is empty or has no type before its name (`int f(int)`, `int f(int x,)`).
- A signature with no parameters is allowed, but tests.txt has no way yet to
  write a case with no inputs; milestone M1 adds one.

### Statement

Everything after the `---` line, as markdown (below). It is stored as
written, except that a `\r` before a newline is dropped; `statement_line`
records which line of `problem.md` it starts on.

## tests.txt

Test cases. M0 parses the whole file but only shows the cases marked
`@example`; later milestones run all of them.

```
# Comments start with '#'.
@example 4 + 9 = 13, so the answer is the indices of 4 and 9.
[4,1,9]
13
=> [0,2]

@example
[5,5]
10
=> [0,1]

[1,2,3,4]
7
=> [2,3]
```

- **A case** is one or more input lines followed by a line starting with `=>`.
  Each input line holds one literal, in the order of `lc_problem.arg_names`.
  The text after `=>` (spaces trimmed) is the expected output, and must not be
  empty.
- **Literals** are kept as raw text in M0: leading and trailing spaces and tabs
  are trimmed, nothing else is checked. (M1 parses them: integers, floats,
  `true`, `false`, `null`, strings with escapes, nested arrays.)
- **`@example`** on its own line marks the next case as an example. Any text
  after `@example` and a space or tab is that example's explanation, shown as
  plain text.
- **Every line is trimmed** of spaces and tabs first, so `#`, `@example` and
  `=>` may be indented.
- **Blank lines and `#` comment lines** are ignored between cases. A comment
  line inside a case is ignored too, but a blank line inside a case (after an
  input line and before its `=>`) is an error, because it nearly always means
  a missing `=>`.
- **Errors**, each reported as `<path>:<line>: <message>`:
  - a blank line inside a case;
  - a `=>` with no input lines before it;
  - a `=>` with nothing after it;
  - an `@example` that is not followed by a case, or two in a row;
  - a line starting with `@` other than `@example`;
  - a case still open at the end of the file;
  - a NUL byte.
- A `\r` before each newline is ignored, and so is a UTF-8 byte order mark.

## Statement markdown

Statements use a small markdown subset. The core parses it into a flat list of
blocks (`lc_md_parse` in `include/lc.h`); front-ends render the blocks. The
parser accepts any input: what isn't recognized below is plain text.

### Blocks

- **Paragraph:** consecutive non-blank lines. Line breaks inside a paragraph
  become spaces. A line ending in two or more spaces, or in a backslash, makes
  a hard line break.
- **Heading:** `#` to `######`, then a space or tab, then the text. Trailing
  `#` characters preceded by a space are removed. A heading is a single line.
- **Fenced code block:** a line starting with three or more backticks (or
  tildes), optionally followed by an info string such as `text` or `cpp`. It
  runs until a line with a fence of the same character at least as long, or
  to the end of the document. Content is verbatim. The fence may be indented
  up to three spaces; that much indentation is removed from content lines.
- **List item:** `-`, `*` or `+` followed by a space (bulleted), or one to
  nine digits then `.` or `)` and a space (numbered). The lines after it, up
  to a blank line or the start of another block, continue the item's text. A
  blank line ends the item: whatever follows starts a new block, and its
  indentation doesn't matter except for list markers. Indenting a marker by
  two or more spaces more than the enclosing item's marker nests it one level
  deeper (`depth`). Numbered items count up from the number of the first item
  in their list, where a list is a run of numbered items at the same depth
  with only deeper items or blank lines between them.
- **Block quote:** lines starting with `>`. The text is inline only (block
  syntax inside a quote stays plain text). A blank `>` line splits it into
  separate quote paragraphs, and a line without `>` ends it.
- **Thematic break:** a line of three or more `-`, `*` or `_` characters, with
  optional spaces between them.
- **Examples placeholder:** a line containing only `@examples` (spaces around
  it allowed), outside a code block. It marks where a front-end shows the
  pack's `@example` cases. A statement without it gets the examples after
  the statement.

Headings, block quotes and thematic breaks may be indented up to three
spaces, like fences. List markers may be indented any amount (that sets
`depth`). A list item's text is inline only, so `- - x` is one item whose
text is `- x`.

Headings, fences, thematic breaks, block quotes, bulleted items, numbered
items starting at 1 and the examples placeholder can start right after a
paragraph line, without a blank line between them. A numbered item with any
other number continues the paragraph.

Not supported: indented code blocks (indent four spaces and you get a
paragraph), setext headings, tables, HTML, images and reference links.

### Inline

- `` `code` ``: a code span. A run of N backticks closes only at a run of
  exactly N backticks. Its content is verbatim, except that line breaks
  become spaces and one space at each end is removed when both ends have one
  (and the content isn't all spaces).
- `**strong**` or `__strong__`, and `*em*` or `_em_`. They nest. An opening
  `*` or `_` must be followed by a non-space character and a closing one must
  follow a non-space character. `_` does not open or close inside a word, so
  `snake_case_name` stays plain. Unmatched markers are plain text.
- `[text](url)`: a link. The text may contain emphasis and code. The url may
  not contain spaces, and link titles are not supported.
- A backslash before an ASCII punctuation character makes it literal.
- Anything else, including `<`, `>`, `&` and non-ASCII UTF-8, is plain text.

### Example statement

This one is made up, to show the pieces together:

````
You are given `heights`, the heights of a row of steps from left to right.
A step is a **rise** when it is taller than the step just before it.

Return how many rises the row has. The first step is never a rise, because
nothing comes *before* it.

@examples

## Constraints

- `1 <= heights.length <= 10^5`
- `0 <= heights[i] <= 10^9`

## Follow-up

Can you count them in one pass, using `O(1)` extra memory?
````
