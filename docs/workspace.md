# Solving problems: the workspace, lc test and lc submit

This is how you write and judge solutions with `lc`. How a solution is
built and run, the limits and the verdicts are in
[judging.md](judging.md); the JSON output is in [json.md](json.md).

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

The editor is run through `/bin/sh` in the problem's directory, with the
solution file as its argument, so `EDITOR='nvim -O'` and the like work.
Without `--lang`, `lc start` uses the language of the solution already
there, if there is exactly one, else C++.

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

## Languages and standards

C solutions are built as C11 and C++ solutions as C++11, as on LeetCode by
default. To use a newer standard:

```sh
lc test --std c++17               # this run only
LC_CXXSTD=c++20 lc test           # every run; --std wins over it
lc test --std max                 # the newest the compiler has
LC_CSTD=c17 lc test solution.c    # C: c99, c11, c17, c23 or max
```

C++ accepts `c++11`, `c++14`, `c++17`, `c++20`, `c++23` and `max`. A
standard the compiler doesn't have is reported as such, not as a puzzling
compile error.

The compilers are `$CC` and `$CXX`, else `cc` and `c++`. A value may
include arguments, as in `CXX='ccache g++'`.

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

## Neovim, and other editors

`lc` is designed to work from Neovim in layers, each working without the
next. M3 has the first two; the plugin comes in a later milestone.

**clangd.** A solution file has no `#include`s, so clangd would see
`vector` and `uint32_t` as errors. `lc start` writes `~/lc/.clangd`,
which tells clangd to read lc's prelude first and which standard to use,
for C and C++ files separately. Completion, diagnostics and go to
definition then work with Neovim's built-in LSP client and nothing else.
`lc start` rewrites `.clangd` when the standard (`LC_CSTD`,
`LC_CXXSTD`) changes. A `.clangd` that `lc` didn't write is left alone;
to keep your own changes to lc's, delete its first line.

**`:make`.** In Vim or Neovim, with no plugin:

```vim
:set makeprg=lc\ test\ %
:make
```

Compile errors and failed cases fill the quickfix list, because they are
printed as `file:line:` messages that the default `errorformat` reads.
`:set makeprg=lc\ submit\ %` does the same for a submission.

**Other front-ends** run `lc test --json` or `lc submit --json`, which
print one JSON object per event as it happens ([json.md](json.md)).
