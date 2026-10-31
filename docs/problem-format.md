# Problem pack format

A problem pack is a directory holding one problem. This document describes
the parts `lc` reads: `problem.md` with its typed signature, `tests.txt`
and the literals its cases are written in, the reference solution, and the
markdown subset used for statements. How to write a pack, with `gen.c`,
`brute.c`, `MANIFEST.tsv` and `lc check`, is in
[authoring.md](authoring.md); `checker.c` and the other files listed in
`docs/PLAN.md` §5.5 come in later milestones. How values travel between
`lc` and a solution's harness is in [wire.md](wire.md); how solutions are
built and judged is in [judging.md](judging.md).

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
  tests.txt      test cases; lc show shows the ones marked @example
  ref.c          the reference solution (or ref.cpp)
  brute.c        optional: a slow, obviously right solution (or brute.cpp)
  gen.c          optional: makes random cases (authoring.md)
```

**The reference solution** is written like any solution: just the
function (or, in C++, the `Solution` class), with no `#include`s, as in
the stub `lc start` writes. It must pass every case of the pack. `lc test`
runs it to work out the expected result of a case of your own that doesn't
give one (see [workspace.md](workspace.md)). Everything shipped must build
as C11 or C++11, so that it builds whatever standard a user picks.

**Name.** The directory is named `NNNN-slug`: the problem ID padded with zeros
to at least four digits (`printf("%04d-%s", id, slug)`), a dash and the slug.
Both must match `problem.md`.

**Which entries are packs.** Every entry of the problems directory whose name
starts with an ASCII digit and is a directory is loaded as a pack. Everything
else (files such as `MANIFEST.tsv`, directories starting with a letter or a
dot) is ignored by the loader; `MANIFEST.tsv` is read by `lc check` and
`lc stats --manifest` ([authoring.md](authoring.md#manifesttsv)).

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
- A file containing a NUL byte is an error, and so is a `problem.md` larger
  than 16 MiB.

### Fields

| Key               | Required | Meaning |
|-------------------|----------|---------|
| `id`              | yes      | the LeetCode number, a decimal integer from 1 to 99999 |
| `slug`            | yes      | lowercase letters and digits in groups joined by single dashes: `two-sum`, `3sum`, `2-keys-keyboard` |
| `title`           | yes      | non-empty |
| `difficulty`      | yes      | `easy`, `medium` or `hard` (lowercase) |
| `tags`            | no       | comma-separated; each tag uses lowercase letters, digits and dashes. Empty entries (`a,,b` or a trailing comma) are errors |
| `signature`       | no       | e.g. `int[] twoSum(int[] nums, int target)`; see below |
| `hidden`          | no       | extra inputs that only shape test data, e.g. `int pos`; see below |
| `check`           | no       | for a problem solved in place: `param NAME` or `prefix NAME`, a parameter that is the result; see below |
| `compare`         | no       | how a result is checked: `exact`, `float [TOL]`, `unordered LEVELS` or `checker`; see below |
| `time_limit_ms`   | no       | positive integer, default 1000 |
| `memory_limit_mb` | no       | positive integer, default 256 |

Any other key (`class`, `api`, ...) is kept as-is for later milestones
and not interpreted yet. Every field, known or not, is available
through `lc_problem.fields` and `lc_problem_field()`.

### Signature and hidden inputs

```
signature: int[] twoSum(int[] nums, int target)
hidden: int pos
```

- **`signature`** is a return type, the function's name, then the
  parameters in parentheses, each a type and a name, separated by commas.
  Types are the names in [Types](#types); a return type may also be `void`.
  Spaces around the parts don't matter, and none is needed after `]`
  (`int[]nums`). Nothing may follow the `)`. `f()` has no parameters.
- **`hidden`** lists extra inputs that only shape test data and are never
  passed to the solution, such as where a list's cycle starts, in the same
  form as parameters: `int pos`, or `int skipA, int skipB`. An empty value
  means none.
- **Names** are C identifiers. A name is the identifier at the end of its
  parameter (`[A-Za-z_][A-Za-z0-9_]*`, not starting with a digit), and the
  type is everything before it. Names must differ from each other, hidden
  ones included, and some can't be used because the generated C and C++ code
  needs them: keywords of either language (`class`, `new`, `bool`, ...),
  `main`, `Solution`, `ListNode`, `TreeNode`, `Node`, `NULL`, `EOF`, `errno`,
  `stdin`, `stdout`, `stderr`, `returnSize`, `returnColumnSizes`, and names
  starting with `_`, `lc_` or `LC_`. The C code also gives an array
  parameter's size the name `<name>Size` (and a 2D one's row sizes
  `<name>ColSize`), so those can't be other names in the signature, and
  the function can't share a name with a parameter.
- **Argument names.** To show an example as `nums = [1,2], target = 3`, a
  case's inputs are named after the signature's parameters followed by the
  hidden inputs (`lc_problem.arg_names`). Without a `signature` there are no
  names, but `hidden` is still checked.
- **Errors** skip the pack, as for any other field:
  `signature has no parentheses`, `signature: parameter 2 is empty`,
  `signature: parameter 'int' has no name`,
  `signature: unknown type 'vector<int>'`,
  `signature: type 'ListNode[][]' is not supported (too many '[]')`,
  `signature: only a return type can be void`,
  `signature: duplicate parameter name 'x'`,
  `signature: 'class' can't be used as a name`,
  `signature: unexpected 'const' after ')'`,
  `hidden: 'x' is already a parameter of the signature`, and the like.

### compare

How a solution's result is checked against the expected one.

| Value              | Meaning |
|--------------------|---------|
| `exact`            | equal values (the default) |
| `float [TOL]`      | doubles may differ by up to TOL, a positive number (default `1e-5`); everything else must be equal. The difference is absolute, not relative |
| `unordered LEVELS` | the order of the arrays at the listed nesting levels doesn't matter: `unordered 0`, `unordered 1`, `unordered 0,1` |
| `checker`          | the pack's `checker.c` decides (milestone M8) |

- **Default:** `exact`, except that a result made of doubles (`double`,
  `double[]`, `double[][]`) defaults to `float 1e-5`, as on LeetCode.
- **Levels** count array nestings from the outside: in `int[][]`, level 0 is
  the list of rows and level 1 the items of each row. So `unordered 1`
  ignores the order inside each row but keeps the order of the rows
  (LeetCode 2215), and `unordered 0,1` ignores both. Duplicates count: `[1,1,2]`
  does not match `[1,2,2]`. A TreeNode, ListNode or NaryNode is compared
  whole; only arrays of them can be unordered.
- **Checked against the result:** `float` needs a result made of doubles,
  and each level must exist in the result's type (`int[]` has only level
  0). The result is what the function returns, or with `check:` (below)
  the parameter checked, so `compare:` and its default apply to that
  parameter. A `void` return without `check:` has no result: it allows
  only `exact` and `checker`, and such a problem can't be judged.

### check

Some problems are solved **in place**: the function changes an array it
is given, and that array, not what the function returns, is the answer.
`check:` says which parameter that is.

| Value         | The function returns | The result is |
|---------------|----------------------|---------------|
| `param NAME`  | `void`               | NAME, after the call |
| `prefix NAME` | `int`, a count k     | the first k elements of NAME, after the call |

```
signature: void moveZeroes(int[] nums)
check: param nums
```

```
signature: int removeElement(int[] nums, int val)
check: prefix nums
compare: unordered 0
```

- **NAME** is a parameter of the signature (not a `hidden:` input). For
  `param` it is an array of any depth (`int[]`, `char[][]`, ...), and for
  `prefix` a one-dimensional array. Lists and trees can't be checked yet.
- **The result's type** is NAME's type: expected outputs in tests.txt are
  written as NAME's value (`=> [1,3,0,0]`), or for `prefix` as its first k
  elements, so the count is their length (`=> [1,2]`, not `2`). That is
  what LeetCode shows as the output of these problems too.
- **compare:** applies to that value. `unordered 0` with `prefix` accepts
  the first k elements in any order (LeetCode 27); without it, they must
  be in order (LeetCode 26). What lies beyond the first k doesn't matter.
- **A count that doesn't fit**, below 0 or above NAME's length after the
  call, is a Runtime Error: the harness stops with a message such as
  `removeElement returned 6, but nums has 5 elements`. A C++ solution may
  resize the vector (e.g. with `erase`), as on LeetCode; the count is
  checked against its size after the call.
- **Errors** skip the pack: `check: needs a signature`,
  `check: unknown mode 'x' (expected param or prefix)`,
  `check: 'x' is not a parameter of the signature`,
  `check: param needs a function that returns void, not int`,
  `check: prefix needs a function that returns int (the count), not void`,
  `check: param needs an array such as int[] or char[][], but 'n' is int`,
  `check: prefix needs a one-dimensional array such as int[], but 'g' is int[][]`,
  `check: lists and trees can't be checked yet, but 'head' is ListNode`,
  and the like.
- **Writing past the end** of an array is caught in C: the harness gives
  every array one zeroed element more than it holds (so a `char[]` is also
  a NUL-terminated string), and a solution that changes it gets a Runtime
  Error, `duplicateZeros wrote past the end of arr (arr[8])`, as LeetCode
  reports a heap buffer overflow. This holds for every one-dimensional
  array parameter, not only the checked one.
- **A `void` function without `check:`** has nothing to judge: `lc test`
  and `lc submit` say so, and `lc check` reports it as an error, not as a
  problem it can't judge yet.

### Statement

Everything after the `---` line, as markdown (below). It is stored as
written, except that a `\r` before a newline is dropped; `statement_line`
records which line of `problem.md` it starts on.

## Types

The types a signature may use. Each has a literal form for tests.txt (see
[Literals](#literals)).

| Type       | Literal                         | Notes |
|------------|---------------------------------|-------|
| `int`      | `-5`                            | 32-bit signed: -2147483648 to 2147483647 |
| `long`     | `-5`                            | 64-bit signed |
| `uint32`   | `5`                             | 0 to 4294967295 |
| `double`   | `2.5`, `-1e-3`, `2`             | an integer literal is fine; when lc prints the value, `2` becomes `2.0` |
| `bool`     | `true`, `false`                 | |
| `char`     | `"a"`                           | a string of exactly one ASCII character |
| `string`   | `"a b"`                         | UTF-8; no NUL bytes |
| `T[]`      | `[1,2,3]`                       | an array of any type above; `[]` is empty |
| `T[][]`    | `[[1,2],[3]]`                   | arrays nest at most two deep |
| `ListNode` | `[1,2,3]`                       | a linked list of ints; `[]` is an empty list |
| `TreeNode` | `[1,null,2]`                    | a binary tree of ints in level order; `[]` is an empty tree |
| `NaryNode` | `[1,null,3,2,4,null,5,6]`       | a tree whose nodes have any number of children |
| `void`     |                                 | only as a return type |

`ListNode[]`, `TreeNode[]` and `NaryNode[]` are allowed; `ListNode[][]` and
the like are not.

**TreeNode** lists the nodes level by level, left to right, the way
LeetCode does. `null` marks a missing child, and a missing node's own
children are not listed. Each item after the first is the next child slot
(left, then right) of the earliest node that still has a free slot, so an
item with no such node left is an error (`[1,null,null,2]`). The first item
can't be `null`. Trailing `null`s say nothing and are dropped: `[1,null,2,null]`
is the same tree as `[1,null,2]`.

**NaryNode** also uses LeetCode's format: the root, then `null`, then the
children of each node in level order, each node's group of children ended
by `null`. In `[1,null,3,2,4,null,5,6]` node 1 has children 3, 2 and 4,
and node 3 has children 5 and 6. A `null` that would start the children of
a node that doesn't exist is an error, and trailing `null`s are dropped.

## Literals

Test values are written in a small subset of JSON:

- **Integers:** an optional `-`, then digits with no leading zeros (`0`,
  `-12`, not `007` or `+1`).
- **Floats:** an integer followed by a fraction (a point and digits), an
  exponent (`e` or `E`, an optional sign, digits) or both:
  `2.5`, `1e-7`, `-0.0`. `.5`, `1.` and `1e` are errors, as are `NaN` and
  `Infinity`. The decimal point is always `.`, whatever the locale.
- **`true`, `false` and `null`.**
- **Strings** in double quotes, with the escapes `\"`, `\\`, `\/`, `\b`,
  `\f`, `\n`, `\r`, `\t` and `\uXXXX` (a surrogate pair for characters
  beyond U+FFFF). Raw control characters must be escaped, the text must be
  valid UTF-8, and `\u0000` is not allowed.
- **Arrays:** `[`, values separated by commas, `]`. No trailing comma.
  Untyped values may nest at most 32 deep.
- Spaces and tabs may surround any value or punctuation.

A literal is checked against its type as it is parsed, and an error names
the column (counting bytes from 1 at the start of the literal):
`column 4: expected int, got a string`, `column 1: 2147483648 is out of
range for int`, `column 5: expected ',' or ']'`.

**Printing.** Values print back as literals with no spaces: `[1,2]`,
`"a\"b"`, `[1,null,2]`. A double prints as the shortest text that reads
back as the same number, with `.0` added to whole numbers (`2.0`, `0.1`,
`1e+20`). Control characters in strings print as escapes, and, in a
program's result only, bytes that aren't valid UTF-8 print as `\xHH`, and
non-finite doubles as `NaN`, `Infinity` and `-Infinity`. A run's report
prints the expected and actual results this way; its inputs, and the
examples `lc show` shows, appear as written in tests.txt.

## tests.txt

Test cases. `lc show` shows the cases marked `@example`, `lc test` runs
them, and `lc submit` runs every case.

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
- **A case with no inputs**, for a signature with no parameters, has the
  single input line `()`:

  ```
  ()
  => "Hello World"
  ```

- **Literals** are kept as text when the file is read (leading and trailing
  spaces and tabs trimmed). `lc_case_values` parses a case's literals
  against the signature: each input against its parameter (then the hidden
  inputs). The number of inputs must match. The expected output is parsed
  against the result's
  type: the return type, or with `check:` the checked parameter's.
  Without a signature, or for a `void` return without `check:`, values
  are parsed without a type. Its errors look like
  `.../tests.txt:12: target: column 1: expected int, got a string`, where
  the line is the case's first line.
- **`@example`** on its own line marks the next case as an example. Any text
  after `@example` and a space or tab is that example's explanation, shown as
  plain text.
- **`@gen SEED`** or **`@gen SEED small`** on its own line marks the next
  case as made by the pack's `gen.c` from that seed, a decimal number below
  2^64 ([authoring.md](authoring.md#generated-cases)). The case must follow
  right away: a blank line after an `@gen` line with no inputs yet is an
  error, which `lc check --regen` fixes by making the case. Judging treats
  it as any other case. A case can't be both `@gen` and `@example`.
- **Every line is trimmed** of spaces and tabs first, so `#`, `@example`,
  `@gen` and `=>` may be indented.
- **Blank lines and `#` comment lines** are ignored between cases. A comment
  line inside a case is ignored too, but a blank line inside a case (after an
  input line and before its `=>`) is an error, because it nearly always means
  a missing `=>`.
- **Errors**, each reported as `<path>:<line>: <message>`:
  - a blank line inside a case;
  - a `=>` with no input lines before it (write `()` for a case with no
    inputs);
  - `()` together with other input lines in one case;
  - a `=>` with nothing after it;
  - an `@example` that is not followed by a case, or two in a row;
  - an `@gen` with a bad seed, not followed right away by a case, or two
    in a row; `@gen` and `@example` on one case;
  - a line starting with `@` other than `@example` and `@gen`;
  - a case still open at the end of the file;
  - a NUL byte;
  - a file larger than 256 MiB.
- A `\r` before each newline is ignored, and so is a UTF-8 byte order mark.

## Statement markdown

Statements use a small markdown subset. The core parses it into a flat list of
blocks (`lc_md_parse` in `include/lc.h`); front-ends render the blocks. The
parser accepts any input: what isn't recognized below is plain text.

A line ends at `\n`, `\r\n` or a lone `\r`, but line numbers count only
`\n`, as editors do. So a block's `line` plus the problem's `statement_line`,
minus 1, is always its line in `problem.md`.

### Blocks

- **Paragraph:** consecutive non-blank lines. Line breaks inside a paragraph
  become spaces. A line ending in two or more spaces, or in a backslash, makes
  a hard line break.
- **Heading:** `#` to `######`, then a space or tab, then the text. Trailing
  `#` characters preceded by a space are removed. A heading is a single line.
- **Fenced code block:** a line starting with three or more backticks (or
  tildes), optionally followed by an info string such as `text` or `cpp`
  (only its first word is kept, as the block's language). It
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
