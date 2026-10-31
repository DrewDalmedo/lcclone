# Solving problems: the workspace, lc test and lc submit

This is how you write and judge solutions with `lc`, keep track of your
progress, and set `lc` up the way you like. How a solution is built and
run, the limits and the verdicts are in [judging.md](judging.md); the
JSON output is in [json.md](json.md).

## The workspace

`lc start` sets up one directory per problem in your workspace, `~/lc`
(or `$LC_WORKSPACE`):

```
~/lc/
  .clangd                              written by lc, for clangd
  .lcclone/                            the preludes .clangd points at
  0001-two-sum/
    README.md                          the statement and examples (rewritten by lc)
    solution.cpp                       your code: written once, from the stub
    cases.txt                          your own cases: written once
```

- **`README.md`** is the problem's title, statement and examples as
  markdown. `lc start` rewrites it every time, so don't keep notes in it.
- **`solution.c` or `solution.cpp`** starts as the stub: the function to
  write, in LeetCode's form for the language (a function in C, a `class
  Solution` in C++). Once it exists, `lc` never touches it again. You can
  have both, one per language.
- **`cases.txt`** is for test cases of your own (see below). It starts
  with instructions in comments, and is never overwritten either.

The workspace is plain files, so it can live in git.

```sh
lc start 1                  # C++ by default; opens $VISUAL, else $EDITOR, else vi
lc start 1 --lang c         # a C solution next to it
lc start two-sum --no-edit  # just create the files and print the solution's path
```

The editor is the config file's `editor`, else `$VISUAL`, else `$EDITOR`,
else `vi` ([Configuration](#configuration)). It is run through `/bin/sh`
in the problem's directory, with the solution file as its argument, so
`EDITOR='nvim -O'` and the like work. With `open_statement = yes` in the
config, `README.md` is passed too, after the solution: with
`editor = nvim -O` the two open side by side. Without `--lang`, `lc start`
uses the language of the solution already there, if there is exactly one,
else the config's `lang`, else C++.

## Running a solution

```sh
lc test                     # in a problem's directory
lc test solution.cpp        # a file: any .c or .cpp in a directory named like 0001-two-sum
lc test ~/lc/0001-two-sum   # a problem's directory
lc test 1                   # a problem: its directory in the workspace
lc submit                   # every case, stopping at the first failure
```

- **`lc test`** runs the problem's examples, then the cases in `cases.txt`
  next to the solution (or the file given with `--case FILE`). Every case
  runs; the failures are shown at the end.
- **`lc submit`** runs every case of the problem, the hidden ones too, and
  stops at the first that fails, as LeetCode does.
- A directory with both `solution.c` and `solution.cpp` needs `--lang c`
  or `--lang cpp`.

Output looks like this:

```
Building solution.cpp (C++, c++11)
  Example 1   Accepted                  2 ms
  Example 2   Wrong Answer              1 ms
  cases.txt:5 Accepted                  1 ms

solution.cpp:1: Wrong Answer on example 2
  Input:    nums = [6,1,6], target = 12
  Output:   [0,0]
  Expected: [0,2]
  Stdout:
    checking 6

Wrong Answer: 2/3 cases passed
```

Each failure starts with a `file:line:` line, and compile errors are the
compiler's own `file:line:col:` messages, with the line numbers of your
file. So editors that read compiler output can jump to both.

**Exit status:** 0 for Accepted, 1 for any other verdict, 2 for a compile
error, 3 for a usage error or anything else that went wrong, and 130 when
^C stopped the run.

## Your own cases: cases.txt

`cases.txt` is written like a problem's `tests.txt`
([problem-format.md](problem-format.md)): one line per input, in the order
of the function's parameters, in the same literal syntax as the examples.
The expected result is optional:

```
# A case with its expected result:
[5,2,7,9]
11
=> [1,3]

# Without one, the problem's reference solution works it out:
[3,3]
6
```

A case without `=>` ends at a blank line or at the end of the file.
Lines starting with `#` are comments. A case whose input breaks the
problem's constraints may make the reference solution fail; `lc` then
says so instead of guessing.

For a problem solved in place, such as Move Zeroes or Remove Duplicates
from Sorted Array, the result is the array after the call (or its first k
elements, k being what the function returns), so the expected result is
written as that array: `=> [1,2]`, not `=> 2`. The instructions at the top
of `cases.txt` say so for those problems ([problem-format.md](problem-format.md#check)).

## Languages and standards

C solutions are built as C11 and C++ solutions as C++11, as on LeetCode by
default. To use a newer standard:

```sh
lc test --std c++17               # this run only
LC_CXXSTD=c++20 lc test           # every run; --std wins over it
lc test --std max                 # the newest the compiler has
LC_CSTD=c17 lc test solution.c    # C: c99, c11, c17, c23 or max
```

Or set `cxxstd = c++17` (and `cstd`) in the config file, for every run:
`$LC_CXXSTD` wins over it, and `--std` over both.

C++ accepts `c++11`, `c++14`, `c++17`, `c++20`, `c++23` and `max`. A
standard the compiler doesn't have is reported as such, not as a puzzling
compile error.

The compilers are the config's `cc` and `cxx`, else `$CC` and `$CXX`,
else `cc` and `c++`. A value may include arguments, as in
`CXX='ccache g++'`. The config's `cflags` and `cxxflags` are added to the
compiler command for your solutions, for example `cxxflags = -Wall
-D_GLIBCXX_DEBUG`.

**What a solution can use without `#include`:** the C prelude includes the
usual C headers (`stdio.h`, `stdlib.h`, `string.h`, `stdbool.h`,
`stdint.h`, `limits.h`, `math.h`, `ctype.h` and a few more) and
[uthash](https://troydhanson.github.io/uthash/), the hash table macros
(`HASH_ADD_INT`, `HASH_FIND_STR` and the rest) LeetCode's C environment
has. Writing `#include "uthash.h"` yourself works too. The C++
prelude includes the standard headers one by one, plus the C++17 and C++20
ones when the standard has them, and says `using namespace std;`. Both are
in `runtime/`.

**`--sanitize`** builds with AddressSanitizer and UndefinedBehaviorSanitizer
(where the compiler has them), so out-of-bounds accesses, use after free
and undefined behaviour become a Runtime Error with the sanitizer's report.
See [judging.md](judging.md#sanitizers).

## Progress and history

`lc submit` records every submission: your progress on the problem, and a
copy of the solution. `lc test` records nothing.

- **Status.** A problem is *solved* once a submission of it was accepted,
  *attempted* when it was submitted but never accepted, and *todo*
  otherwise. A later failure doesn't undo solved.
- **Progress** is kept in `progress.tsv` in the data directory,
  `$XDG_DATA_HOME/lcclone` (by default `~/.local/share/lcclone`): one
  tab-separated line per problem, with its ID, status, language, best
  time (the slowest case of the fastest accepted submission, not counting
  runs with `--sanitize`) and when it was last submitted, in seconds since
  1970. `lc` reads it again before each update and replaces it atomically,
  under a lock, so two `lc` processes (or a GUI) can record at once. It
  changes only the line of the problem it records: comments and lines of
  your own stay.
- **History.** Each submission is copied to
  `submissions/NNNN/<time>-<verdict>.<ext>` in the same directory, for
  example `submissions/0001/20261030T201503Z-accepted.cpp` (the time is
  UTC).

```sh
lc list --status todo                # what you haven't submitted yet
lc list --status attempted,solved    # the rest
lc random --difficulty easy --status todo
lc stats                             # solved and attempted, by difficulty and tag
```

`lc list` shows a Status column once you have submitted something, and
`lc show` a Status line. `lc random` picks one problem among those that
pass the same filters as `lc list`. `lc stats` counts what you solved and
attempted, in all, by difficulty and by tag.

## Configuration

Settings that you'd otherwise put in variables or repeat on the command
line can go in a config file, `~/.config/lcclone/config` (or
`$XDG_CONFIG_HOME/lcclone/config`, or the file named by `$LC_CONFIG`):

```
# One "key = value" per line; '#' starts a comment line.
cxxstd = c++17
editor = nvim -O
open_statement = yes
workspace = ~/code/lc
```

| Key              | Value |
|------------------|-------|
| `cc`, `cxx`      | the C and C++ compilers, e.g. `ccache g++` |
| `cstd`, `cxxstd` | the standards: `c99` ... `c23`, `c++11` ... `c++23`, or `max` |
| `cflags`, `cxxflags` | extra compiler flags for your solutions |
| `editor`         | the editor `lc start` runs |
| `open_statement` | `yes` to open README.md with the solution in `lc start` |
| `lang`           | the language `lc start` uses when there is no other hint: `c` or `cpp` |
| `color`          | `auto`, `always` or `never`, as `--color` |
| `workspace`      | the workspace directory: an absolute path, or one starting with `~/` |

Spaces around the key and the value don't matter, and an empty value is
the same as leaving the key out. A line `lc` can't use, such as an
unknown key or a bad value, is skipped with a warning naming the file and
line; the rest still applies.

**Which setting wins.** The most specific: a command-line option, then an
`lc` variable (`LC_CXXSTD`, `LC_CSTD`, `LC_WORKSPACE`), then the config
file, then a general variable (`CC`, `CXX`, `VISUAL`, `EDITOR`,
`NO_COLOR`), then the default. So `cxx = g++-14` in the config wins over
a `CXX` set for other programs, and `--std` wins over everything.

## Neovim, and other editors

`lc` is designed to work from Neovim in layers, each working without the
next: clangd and `:make` need nothing but `lc`, and the plugin,
[lc.nvim](https://github.com/DrewDalmedo/lc.nvim), adds the rest.

**clangd.** A solution file has no `#include`s, so clangd would see
`vector` and `uint32_t` as errors. `lc start` writes `~/lc/.clangd`,
which tells clangd to read lc's prelude first and which standard to use,
for C and C++ files separately. Completion, diagnostics and go to
definition then work with Neovim's built-in LSP client and nothing else.
`lc start` rewrites `.clangd` when the standard (`LC_CSTD`,
`LC_CXXSTD`, or `cstd` and `cxxstd` in the config) changes. A `.clangd` that `lc` didn't write is left alone;
to keep your own changes to lc's, delete its first line.

**`:make`.** In Vim or Neovim, with no plugin:

```vim
:set makeprg=lc\ test\ %
:make
```

Compile errors and failed cases fill the quickfix list, because they are
printed as `file:line:` messages that the default `errorformat` reads.
`:set makeprg=lc\ submit\ %` does the same for a submission.

**The plugin.** [lc.nvim](https://github.com/DrewDalmedo/lc.nvim) lives in
its own repository; install it with your plugin manager (with lazy.nvim,
`{ "DrewDalmedo/lc.nvim", opts = {} }`). It gives you `:Lc`:

```vim
:Lc list --status todo     " a picker over the problems
:Lc start 1                " sets up Two Sum, opens the solution beside its statement
:Lc test                   " in the background: progress and result in a window
:Lc submit
:Lc std c++17              " the standard for this Neovim session
```

Compile errors and sanitizer reports become diagnostics on your solution
and fill the quickfix list; in the result window, `a` adds the case under
the cursor to `cases.txt`. It works through `lc ... --json`, so it follows
your config file. `:help lcclone` has the rest.

**Other front-ends** run `lc test --json` or `lc submit --json`, which
print one JSON object per event as it happens ([json.md](json.md)).
