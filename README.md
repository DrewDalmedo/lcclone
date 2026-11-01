# lcclone

lcclone is an offline, LeetCode-style practice tool for Unix terminals. You
browse problems and read their statements in the terminal, write solutions in
C or C++ in your own editor, and run them against local tests to get
LeetCode-style verdicts. There is no network access and no account. Problems
keep LeetCode's numbers, slugs and function signatures, so a solution moves
between the two unchanged, but every statement, example and test case in
this repository is written from scratch. The tool itself is C11 plus
POSIX.1-2008 and needs nothing beyond libc; solutions need a C or C++
compiler.

## Status

Milestones **M0** to **M5** are in, and **M6** is under way.

- **M0:** the core library (`liblc.a`), the problem pack loader, the
  statement markdown parser, and the `lc list` and `lc show` commands, both
  with `--json` output.
- **M1:** typed signatures, test literals, the comparison modes, and the
  wire format used to talk to a solution's harness.
- **M2:** the runner and the judge: one process per test case under CPU,
  memory, stack and output limits, LeetCode's verdicts, progress events and
  cancelling. See [docs/judging.md](docs/judging.md).
- **M3:** solving problems end to end in C and C++. `lc start` sets up a
  problem's directory with the stub, `lc test` runs your solution on the
  examples and your own cases, and `lc submit` on every case. Solutions are
  built with a generated harness and cached; compile errors and failed
  cases come out as `file:line:` messages for your editor, or as JSON
  Lines. clangd and Vim's `:make` work out of the box. See
  [docs/workspace.md](docs/workspace.md).
- **M4:** progress and content tooling. `lc submit` records your progress
  and keeps a copy of each submission; `lc list --status`, `lc random` and
  `lc stats` use it. A config file holds your settings (compilers,
  standards, editor, workspace). For authors, `MANIFEST.tsv` lists the
  problem set, and `lc check` checks a pack: its stubs build, its
  reference solution passes every case, and it agrees with a brute-force
  solution on random cases from the pack's generator; `lc check --regen`
  works out the expected results. See
  [docs/authoring.md](docs/authoring.md).
- **M5:** the Neovim plugin, [lc.nvim](https://github.com/DrewDalmedo/lc.nvim),
  in a repository of its own. `:Lc list` picks a problem, `:Lc start`
  opens it beside its statement, and `:Lc test` and `:Lc submit` run in
  the background with live progress and the result in a window; compile
  errors and sanitizer reports become diagnostics and fill the quickfix
  list, and one key adds a failed case to `cases.txt`. It is built on
  `lc --json`. See
  [docs/workspace.md](docs/workspace.md#neovim-and-other-editors).
- **M6** (under way): every Easy problem that is a plain function. The
  harness feature it needs is in: `check:`, for problems solved in place,
  where the array the function changes (or its first k elements) is the
  answer, as in Move Zeroes or Remove Duplicates from Sorted Array. See
  [docs/problem-format.md](docs/problem-format.md#check). The first batch
  of problems is in: every in-place, bits, floating-point and any-order
  problem, 34 in all. The plain ones, 632, follow in batches of about 25;
  the first of them, the 25 lowest-numbered (13 to 268), is in too.

70 problems are included. All but 104 can be judged; binary trees, linked
lists, design problems and the like come in later milestones
(`docs/PLAN.md` section 9). `lc list` shows them, and
`problems/MANIFEST.tsv` lists every LeetCode Easy problem: 974, of which
751 are in scope. `lc stats --manifest` shows how far the set is.

## Building

You need a C11 compiler and a POSIX `make`, and for `make test` also a C++
compiler (`CXX`, default `c++`): the tests build and judge solutions in
both languages. The targets are Linux
(glibc and musl), macOS, FreeBSD and OpenBSD; CI builds and tests on Linux
with gcc, clang, musl and bmake, and on macOS.

```sh
make            # builds liblc.a and lc
make test       # unit tests, then the end-to-end tests
make check-packs  # lc check over every problem pack
```

The build turns the harness runtime in `runtime/` into C arrays with a
short `awk` script (`tools/embed.awk`), so `lc` carries it.

`CC`, `CFLAGS`, `LDFLAGS` and `PREFIX` can be overridden on the command line,
for example `make CC=clang CFLAGS='-O0 -g'`.

## Trying it from the source tree

Point `LC_PROBLEMS` at the repository's `problems/` directory:

```sh
LC_PROBLEMS=problems ./lc list
```

or export it once and run the other commands:

```sh
export LC_PROBLEMS=$PWD/problems
./lc show 1
./lc show valid-parentheses
./lc list --tag tree
./lc list --json
./lc show 104 --json
```

Then solve one:

```sh
./lc start 1           # creates ~/lc/0001-two-sum/ and opens your editor
cd ~/lc/0001-two-sum
lc test                # the examples and your cases in cases.txt
lc submit              # every case
```

(with `lc` on your `PATH`, or the path to it).

## Commands

```
lc list [--difficulty LIST] [--tag TAG] [--status LIST] [--json] [--color=auto|always|never]
lc random [--difficulty LIST] [--tag TAG] [--status LIST] [--json]
lc show <id|slug> [--json] [--no-pager] [--color=auto|always|never]
lc start <id|slug> [--lang c|cpp] [--no-edit] [--json]
lc test [<solution>] [--lang c|cpp] [--std STD] [--sanitize] [--case FILE] [--json]
lc submit [<solution>] [--lang c|cpp] [--std STD] [--sanitize] [--json]
lc stats [--manifest] [--json]
lc check [<pack>...] [--regen] [--count N] [--seed N] [--json]
lc help [<command>]
lc version
```

- `lc list` lists the problems in ID order. `--difficulty` keeps only the
  given difficulties (a comma-separated list such as `easy,medium`),
  `--tag` only problems with that tag, and `--status` only those with
  these statuses (`todo`, `attempted`, `solved`). Once you have submitted
  something, a Status column shows yours.
- `lc random` picks one of the problems that pass the same filters, for
  example `lc random --difficulty easy --status todo`.
- `lc show` renders a problem's statement and examples in the terminal. A
  problem can be named by its number (`104` or `0104`) or its slug
  (`maximum-depth-of-binary-tree`). When the output is a terminal it goes
  through `$PAGER`; `--no-pager` prints it directly. `--color` chooses whether
  ANSI colors are used.
- `lc start` sets up the problem's directory in your workspace (`~/lc`, or
  `$LC_WORKSPACE`): `README.md` with the statement, the solution file with
  the function to write, and `cases.txt` for cases of your own. Then it
  opens your editor (`$VISUAL`, `$EDITOR` or `vi`).
- `lc test` builds your solution and runs it on the examples and on your
  cases; `lc submit` runs it on every case, stopping at the first failure,
  and then checks an accepted solution with AddressSanitizer and UBSan,
  which LeetCode builds with.
  The solution is a file, a problem's directory, a problem's ID or slug
  (its directory in the workspace), or by default the current directory.
  They exit with 0 for Accepted, 1 for another verdict, 2 for Compile
  Error and 3 for an error. `--std` raises the C++ standard from C++11
  (or the C standard from C11), and `--sanitize` builds with
  AddressSanitizer and UBSan. Details in
  [docs/workspace.md](docs/workspace.md).
- `lc submit` records each submission: your status on the problem
  (solved once one was accepted, else attempted), your best time, and a
  copy of the solution. `lc stats` counts what you have solved by
  difficulty and tag. See
  [docs/workspace.md](docs/workspace.md#progress-and-history).
- `lc check` is for problem authors: it checks problem packs, and with
  `--regen` rewrites their `tests.txt`
  ([docs/authoring.md](docs/authoring.md)). `lc stats --manifest`
  reports how much of the problem set in `MANIFEST.tsv` is written.
- `--json` prints machine-readable output instead. The format is versioned
  and described in [docs/json.md](docs/json.md), so editor plugins and GUIs
  can build on it.
- `lc help` describes the commands and `lc version` prints the version.

## Configuration

Settings go in `~/.config/lcclone/config` (or
`$XDG_CONFIG_HOME/lcclone/config`, or `$LC_CONFIG`), one `key = value`
per line:

```
cxxstd = c++17
editor = nvim -O
open_statement = yes
```

The keys are `cc`, `cxx`, `cstd`, `cxxstd`, `cflags`, `cxxflags`,
`editor`, `open_statement`, `lang`, `color` and `workspace`. A command-line
option wins over an `LC_` variable, which wins over the config file, which
wins over a general variable such as `CXX` or `EDITOR`. Details in
[docs/workspace.md](docs/workspace.md#configuration).

## Installing

```sh
make install                                # lc in /usr/local/bin, packs in /usr/local/share/lcclone/problems
make PREFIX=$HOME/opt/lcclone install       # a per-user install; put $HOME/opt/lcclone/bin on your PATH
make PREFIX=/usr DESTDIR=/tmp/stage install # a staged install, for packaging
make PREFIX=$HOME/opt/lcclone uninstall     # uninstall with the same PREFIX (and DESTDIR)
```

`make install` copies `lc` to `$(PREFIX)/bin` and this repository's
`problems/` to `$(PREFIX)/share/lcclone/problems`. `PREFIX` is compiled into
`lc` so that it can find the installed packs; changing it rebuilds what
depends on it. `DESTDIR` is only prepended when copying and is not compiled
in. The installed files are readable by everyone whatever your umask.
`make uninstall` removes `lc` and the packs this repository ships, and
nothing else.

Avoid `PREFIX=$HOME/.local`. It would put the bundled packs in
`~/.local/share/lcclone`, which is your own data directory: `lc` looks for
your own packs there (see below), and keeps your progress and submission
history there. `make install` would copy the bundled packs over
your packs of the same name.

## Where problems are found

`lc` uses the first of these that applies:

1. `$LC_PROBLEMS`, if set (it must be a directory);
2. `$XDG_DATA_HOME/lcclone/problems` (by default
   `~/.local/share/lcclone/problems`), if it exists;
3. `$(PREFIX)/share/lcclone/problems`, if it exists.

Each problem is a directory `NNNN-slug` holding `problem.md` (a header and the
statement) and `tests.txt` (the test cases). The format is specified in
[docs/problem-format.md](docs/problem-format.md). Statements, examples and
tests must be original: LeetCode's text and test data are copyrighted, so
nothing is copied from it.

## Third-party code

`runtime/c/uthash.h` is [uthash](https://troydhanson.github.io/uthash/)
2.3.0 by Troy D. Hanson, under the BSD license at the top of the file. It
is included unchanged, so that C solutions can use it as on LeetCode.

## Credits

The list of problems in `problems/MANIFEST.tsv` (numbers, slugs, titles,
difficulty and premium flags) comes from the problem index of
[doocs/leetcode](https://github.com/doocs/leetcode) by the doocs
contributors, licensed under
[CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/);
`MANIFEST.tsv` is shared under the same license. See
[docs/authoring.md](docs/authoring.md#how-manifesttsv-was-built).

## Repository layout

```
include/lc.h            public API of the core library
src/core/               liblc.a: context and errors, arena, strings, files,
                        pack loader, tests.txt parser, markdown parser, JSON writer,
                        types and signatures, values, comparison, wire format,
                        runner (processes and limits), judge (verdicts and events),
                        languages and standards, code generation, building,
                        the workspace, the config file, progress and history,
                        MANIFEST.tsv, lc check
src/cli/                lc: argument handling, the commands, ANSI output, pager
runtime/                the preludes and harness runtimes built with every
                        solution, and the generator runtime (runtime/gen/)
tools/embed.awk         turns runtime/ into C arrays at build time
problems/               problem packs, one NNNN-slug directory each, and
                        MANIFEST.tsv
tests/unit/             unit tests, one program per suite
tests/fixtures/         fixture packs used by the tests: misbehaving programs for
                        the runner (run/), solutions for every verdict and type
                        (judge/), signatures for code generation (codegen/),
                        C++ solutions of the shipped problems (solutions/), and
                        packs for lc check (check/)
tests/expected/         golden files: the e2e tests' output, generated code
tests/e2e.sh            end-to-end tests of the lc binary
docs/PLAN.md            design, architecture and milestones
docs/workspace.md       solving problems: lc start, test and submit, editors,
                        progress, the config file
docs/authoring.md       writing packs: gen.c, brute.c, lc check, MANIFEST.tsv
docs/problem-format.md  the problem pack format, types and test literals
docs/wire.md            the wire format between lc and a solution's harness
docs/judging.md         building, running, the limits, and the verdict rules
docs/json.md            the --json output format
```
