# lcclone: plan

An offline, terminal-based LeetCode-style practice tool for Unix. You browse
problems, write solutions in your own editor, run them against tests locally,
get LeetCode-style verdicts, and track progress. There is no network and no
account.

## 1. Goals and constraints

- **Language:** C11 plus POSIX.1-2008 (`_POSIX_C_SOURCE 200809L`). Only libc.
  No ncurses, no JSON, sqlite or regex libraries.
- **Build:** a plain POSIX `Makefile` and any `cc` (gcc, clang, tcc).
- **Platforms:** Linux (glibc and musl), macOS, FreeBSD and OpenBSD.
- **Output:** one binary, `lc`, plus data files (problem packs and
  per-language harness runtimes).

Not in v1:
- network sync
- importing problems from LeetCode
- a web UI
- contests
- a sandbox against hostile code

You run your own code, so resource limits only guard against accidents like
infinite loops and memory blowups. They are not a defense against attackers.

### Content and licensing

LeetCode's problem statements and test data are copyrighted, and their terms
forbid scraping. So the project:
- ships **original** problems: classic algorithm tasks with statements and
  tests written from scratch;
- documents the problem format so anyone can write their own;
- does not include a scraper or importer.

## 2. User experience

```
$ lc list --difficulty easy
   #  Title            Diff   Tags              Status
   1  Pair Sum         easy   array, hash       solved
   2  Reverse List     easy   linked-list       -
$ lc show 1                 # statement rendered in the terminal, via $PAGER
$ lc start 1 [--lang c]     # writes ~/lc/0001-pair-sum.c from a stub, opens $EDITOR
$ lc test 1                 # runs the example cases only
$ lc test 1 --case my.txt   # custom input; expected output comes from the reference solution
$ lc submit 1               # all cases, including hidden ones; records the result
$ lc stats                  # solved counts by difficulty and tag
$ lc random --difficulty medium
$ lc check problems/0001-pair-sum   # validates a problem pack (for authors)
```

The verdicts are Accepted, Wrong Answer, Compile Error, Runtime Error (with the
signal or exit code and the tail of stderr), Time Limit Exceeded, Memory Limit
Exceeded and Output Limit Exceeded.

When a case fails, `lc` shows:
- the case number;
- its input, the expected result and your result;
- anything your code printed (LeetCode's "stdout" pane).

## 3. Architecture

```
problem pack (text files)
        |
lc CLI -+- problem loader -- value parser -- codegen --> stub + harness source
        |                                                     |
        |                              compile ($CC) <--------+
        |                                    |
        +- judge -- runner (fork/exec, rlimits, poll, timeouts) -- compare -- verdict -- store
```

**Key idea: the C tool does all the parsing of LeetCode-syntax values.** Before
a run, it turns each test input (`[2,7,11,15]`) into a simple, length-prefixed
**wire format** and writes it to the program's stdin. The per-language harness
only has to:
1. read the wire format;
2. call your function;
3. write the result back in wire format on **fd 3**.

The tool parses that result and compares it structurally with the expected
value.

This has two benefits:
- **Small harnesses.** Each language harness stays small (about 150 lines), so
  adding C++ or Python later is cheap.
- **Clean output.** Your own prints stay on stdout and stderr, apart from the
  result.

### 3.1 Function-signature model (like LeetCode)

Each problem declares a typed signature, for example
`int[] pairSum(int[] nums, int target)`.

These are the v1 types:

```
int  long  double  bool  char  string
int[]  long[]  double[]  bool[]  char[]  string[]
int[][]  char[][]  string[][]
ListNode  TreeNode  void
```

Stubs for C follow LeetCode's own C conventions, so they feel familiar:

| Spec type  | C parameter                                      | C return                                                  |
|------------|--------------------------------------------------|-----------------------------------------------------------|
| `int`      | `int x`                                          | `int`                                                     |
| `long`     | `long long x`                                    | `long long`                                               |
| `bool`     | `bool b`                                         | `bool`                                                    |
| `string`   | `char* s`                                        | `char*`                                                   |
| `int[]`    | `int* nums, int numsSize`                        | `int* f(..., int* returnSize)`                            |
| `int[][]`  | `int** grid, int gridSize, int* gridColSize`     | `int** f(..., int* returnSize, int** returnColumnSizes)`  |
| `string[]` | `char** strs, int strsSize`                      | `char** f(..., int* returnSize)`                          |
| `char[][]` | `char** board, int boardSize, int* boardColSize` | `char** f(..., int* returnSize, int** returnColumnSizes)` |
| `ListNode` | `struct ListNode* head`                          | `struct ListNode*`                                        |
| `TreeNode` | `struct TreeNode* root`                          | `struct TreeNode*`                                        |

The generated stub for Pair Sum looks like this:

```c
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* pairSum(int* nums, int numsSize, int target, int* returnSize) {

}
```

**Prelude.** Your C file doesn't need any `#include`s, just like on LeetCode.
The tool builds one translation unit from three parts:
1. `#include "lc_prelude.h"`, which brings in stdio, stdlib, string, stdbool,
   limits, math and the ListNode/TreeNode structs;
2. `#line 1 "<your file>"`;
3. your source.

Compiler errors then point at your file's real line numbers. `#line` is
portable, unlike gcc's `-include`.

**uthash.** We optionally vendor `uthash.h` (a single BSD-licensed header) into
`runtime/c/`, because LeetCode's C environment provides it and many C solutions
rely on it.

### 3.2 Wire format

Tokens are separated by whitespace. Strings are length-prefixed, so they can
contain spaces and newlines.

| Type       | Literal (tests file) | Wire                                |
|------------|----------------------|-------------------------------------|
| `int`      | `-5`                 | `-5`                                |
| `bool`     | `true`               | `1`                                 |
| `double`   | `2.5`                | `2.5`                               |
| `string`   | `"a b"`              | `3:a b`                             |
| `int[]`    | `[2,7,11]`           | `3 2 7 11`                          |
| `int[][]`  | `[[1,2],[3]]`        | `2 2 1 2 1 3`                       |
| `string[]` | `["ab","c"]`         | `2 2:ab 1:c`                        |
| `ListNode` | `[1,2,3]`            | `3 1 2 3`                           |
| `TreeNode` | `[1,null,2]`         | `3 1 n 2` (level order, `n` = null) |

### 3.3 Checking results

**`check:`** says what to compare:
- `return` (the default);
- `param nums`, for `void` functions that modify their input;
- `prefix nums`, for "return k and the first k elements of nums" problems such
  as remove-duplicates.

**`compare:`** says how to compare:
- `exact` (the default);
- `unordered`: sort the top level before comparing;
- `unordered-deep`: sort at every level, as in group-anagrams;
- `float 1e-5`;
- `checker`: a small C program shipped with the problem, for problems with many
  valid answers.

### 3.4 Problem pack format

Each problem is a directory of plain text files:

```
problems/0001-pair-sum/
  problem.md    header, then the statement (a markdown subset)
  tests.txt     test cases
  ref.c         reference solution: validates the pack and computes expected output for custom input
  checker.c     optional
```

Here is an example `problem.md`:

```
id: 1
slug: pair-sum
title: Pair Sum
difficulty: easy
tags: array, hash-table
signature: int[] pairSum(int[] nums, int target)
compare: unordered
time_limit_ms: 1000
memory_limit_mb: 256
---
Given an integer array `nums` and an integer `target`, return the indices of
the two numbers that add up to `target`. ...

## Constraints
- `2 <= nums.length <= 10^4`
```

And an example `tests.txt`. Each case has one line per parameter, then
`=> expected`, with a blank line between cases. Cases marked `@example` are
shown by `lc show` and run by `lc test`.

```
@example
[2,7,11,15]
9
=> [0,1]

[3,3]
6
=> [0,1]
```

Both files are line-based and parsed by hand: the header is `key: value` pairs
up to `---`. A value literal is a small JSON subset (integers, floats,
true/false, null, strings with escapes, nested arrays), so the recursive-descent
parser is about 300 lines.

### 3.5 Runner

v1 starts **one process per test case**. That keeps things simple, shows
exactly which case failed, and global state can't leak between cases. A batch
mode can come later if start-up cost matters (mostly for Python).

1. Create pipes for stdin, stdout, stderr and the fd 3 result, then `fork()`.
2. **In the child:**
   - `setpgid(0,0)`;
   - set `setrlimit` limits:
     - `RLIMIT_CPU` = limit + 1s;
     - `RLIMIT_AS` = memory limit (skipped under ASan, and on macOS where it
       isn't enforced);
     - `RLIMIT_STACK` = 256 MB, for deep recursion;
     - `RLIMIT_FSIZE` small;
     - `RLIMIT_CORE` = 0;
   - `chdir` to a scratch directory, `dup2` the pipes, and `execv`.
3. **In the parent:**
   - run a `poll()` loop over every pipe *in both directions* (writing all of
     stdin before reading can deadlock on big inputs);
   - enforce a wall-clock deadline with `CLOCK_MONOTONIC`;
   - cap captured output at about 1 MB, which gives Output Limit Exceeded;
   - when the deadline passes, run `kill(-pgid, SIGKILL)`.
   - Ignore `SIGPIPE` and handle `EPIPE`, because the child may exit without
     reading its input.
4. **Collect the result.** `wait4()` gives the exit status, CPU time
   (`ru_utime + ru_stime`) and peak RSS. `ru_maxrss` is in KB on Linux and the
   BSDs but in bytes on macOS, so it has to be normalized. These values map to
   a verdict.

### 3.6 Building solutions

- **Default command:** `$CC -std=c11 -O2 prelude+solution.c harness.c lc_rt.c -lm`.
- **Cache:** builds go in `$XDG_CACHE_HOME/lcclone/build/<slug>/<lang>/`, and a
  build is redone only when an input is newer than the binary.
- **`--sanitize`:** adds `-g -fsanitize=address,undefined`, which is what
  LeetCode does for C and C++. Under ASan the `RLIMIT_AS` limit is turned off.
- **Language table (`lang.c`):** each entry has a name, a file extension, a
  stub generator, a harness generator, a compile command (empty for
  interpreted languages) and a run command. A language counts as available only
  if its tool is on `PATH`.
- **Roadmap:** v1 supports C, then C++ (`c++ -std=c++17`), then Python 3.

### 3.7 Storage (XDG)

| What            | Where                                                                   |
|-----------------|-------------------------------------------------------------------------|
| Problem packs   | `$LC_PROBLEMS`, then `~/.local/share/lcclone/problems`, then `$(PREFIX)/share/lcclone/problems` |
| Your solutions  | `~/lc/` by default (`workspace =` in config), so you can keep it in git |
| Progress        | `~/.local/share/lcclone/progress.tsv`: `id status lang best_ms last_ts` |
| History         | `~/.local/share/lcclone/submissions/<id>/<timestamp>-<verdict>.<ext>`   |
| Config          | `~/.config/lcclone/config`, `key = value` (editor, lang, cc, cflags, color, workspace) |

Every write is atomic: the data goes to a temp file, then `rename()` moves it
into place.

### 3.8 Terminal rendering

Statements use a markdown subset: headings, paragraphs, inline code, code
blocks, bullet lists and bold.
- **Colors:** rendered with plain ANSI SGR codes, without terminfo. Colors are
  used only when stdout is a tty and `NO_COLOR` is unset.
- **Paging:** long output goes through `$PAGER` (default `less -R`).

## 4. Source layout

```
Makefile              POSIX make; PREFIX/DESTDIR install; `make test`
src/
  main.c              argument parsing (POSIX getopt), subcommand dispatch
  cmd_*.c             one file per subcommand: list, show, start, test, submit, stats, random, check
  problem.[ch]        problem.md / tests.txt parsing; index and lookup by id or slug
  sig.[ch]            signature parsing and the type table
  value.[ch]          LeetCode literal parse/print; compare modes
  wire.[ch]           value <-> wire format
  lang.[ch]           language table
  codegen.[ch]        stub and harness generation per language
  build.[ch]          compiling with the cache
  runner.[ch]         fork/exec, rlimits, poll loop, rusage
  judge.[ch]          runs cases, compares, builds the verdict report
  store.[ch]          XDG paths, config, progress, submission history
  render.[ch]         markdown subset -> ANSI; pager
  util.[ch]           arena allocator, string builder, file I/O, atomic write, path join
runtime/
  c/                  lc_prelude.h, lc_rt.c (wire I/O, ListNode/TreeNode builders), uthash.h
  cpp/ python/        later
problems/             the bundled original problem pack
tests/
  unit/               C unit tests (a tiny assert macro, no framework)
  fixtures/           misbehaving programs and fixture problems
  e2e.sh              POSIX sh end-to-end verdict checks
```

**Memory:** each command gets one arena allocator. The process is short-lived,
so nothing is freed piece by piece. The only global mutable state is the
loaded config.

**Size:** the core should come to roughly 4–6k lines of C, plus the runtimes.

## 5. Testing

- **Unit tests** (`make test`):
  - value parse/print round trips;
  - each compare mode;
  - the wire conversion;
  - the problem and signature parsers.
- **Runner tests:** fixture programs that each misbehave in one way, each with
  the verdict it should get:

  | Fixture program       | Expected verdict           |
  |-----------------------|----------------------------|
  | spins forever         | Time Limit Exceeded        |
  | segfaults             | Runtime Error              |
  | allocates 2 GB        | Memory Limit Exceeded      |
  | prints 100 MB         | Output Limit Exceeded      |
  | exits with code 3     | Runtime Error              |
  | closes stdin early    | no hang or SIGPIPE in `lc` |
  | forks a child         | process group is killed    |
- **End to end** (`tests/e2e.sh`): correct, wrong, slow and crashing solutions
  to fixture problems should each get the expected verdict.
- **Pack validation:** `lc check` parses each header, checks every case
  against the declared types, and requires the reference solution to pass every
  case. CI runs it over all the bundled problems.
- **CI:** GitHub Actions on Ubuntu (gcc and clang), Alpine (musl) and macOS.
  Dev builds use `-Wall -Wextra -Wpedantic -Werror`, and the test build uses
  ASan and UBSan.

## 6. Milestones

| #  | Milestone        | Scope                                                                                     |
|----|------------------|-------------------------------------------------------------------------------------------|
| M0 | Skeleton         | Makefile, CLI dispatch, arena/strbuf/file utilities, problem loader, `list`, `show`       |
| M1 | Values           | literal parser and printer, compare modes, wire format; unit tests                        |
| M2 | Runner           | fork/exec, limits, poll loop, verdicts; tests with misbehaving programs                    |
| M3 | C end to end     | stub and harness generation for scalars, arrays and strings; C runtime; `start`, `test`, `submit`; 5 problems. **First usable version.** |
| M4 | Progress         | store, `stats`, `random`, submission history, config file                                 |
| M5 | Types and content| ListNode, TreeNode, 2D arrays, in-place checks, `lc check`, about 30 original problems    |
| M6 | More languages   | C++ and Python 3 harnesses; custom checkers; large generated tests to catch slow solutions |

Possible later work:
- "Design" problems, such as an LRU cache. These need a different harness:
  one class plus a list of method calls.
- A full-screen browser written with raw termios and ANSI codes (no ncurses).
- A batch mode for the runner.
- Linux-only hardening (namespaces, seccomp) behind `#ifdef`.

## 7. Risks

- **Codegen grows with types × languages.** To contain it, keep the wire
  format simple and let one type table drive every generator.
- **Timing noise.** Judge on CPU time rather than wall time, and set time
  limits generously (about 3× the reference solution's time on the large
  cases).
- **Weak memory limits on macOS.** Rely on `ru_maxrss` after the run there.
- **ASan conflicts with `RLIMIT_AS`.** Turn the address-space limit off when
  sanitizing.
- **Content is the bottleneck.** Writing good original problems with strong
  tests takes longer than writing the tool. `lc check` and test generators
  (`gen.c` plus the reference solution) make it cheaper.

## 8. Open questions

1. **Languages:** which ones do you want for solutions? The plan assumes C
   first, then C++ and Python.
2. **Interface:** is a CLI plus `$EDITOR` enough, or do you want a full-screen
   TUI early?
3. **Problems:** which topics should the first problem set focus on?
