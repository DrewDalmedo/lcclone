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

Milestones **M0** to **M3** are in. This is the first usable version.

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

Eleven problems are included: 1, 9, 14, 20, 70, 104, 121, 190, 643, 744
and 867. All but 104 can be judged; binary trees, linked lists, design
problems and the like come in later milestones (`docs/PLAN.md` section 9).

## Building

You need a C11 compiler and a POSIX `make`, and for `make test` also a C++
compiler (`CXX`, default `c++`): the tests build and judge solutions in
both languages. The targets are Linux
(glibc and musl), macOS, FreeBSD and OpenBSD; CI builds and tests on Linux
with gcc, clang, musl and bmake, and on macOS.

```sh
make            # builds liblc.a and lc
make test       # unit tests, then the end-to-end tests
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
lc list [--difficulty LIST] [--tag TAG] [--json] [--color=auto|always|never]
lc show <id|slug> [--json] [--no-pager] [--color=auto|always|never]
lc start <id|slug> [--lang c|cpp] [--no-edit] [--json]
lc test [<solution>] [--lang c|cpp] [--std STD] [--sanitize] [--case FILE] [--json]
lc submit [<solution>] [--lang c|cpp] [--std STD] [--sanitize] [--json]
lc help [<command>]
lc version
```

- `lc list` lists the problems in ID order. `--difficulty` keeps only the
  given difficulties (a comma-separated list such as `easy,medium`) and
  `--tag` only problems with that tag.
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
  cases; `lc submit` runs it on every case, stopping at the first failure.
  The solution is a file, a problem's directory, a problem's ID or slug
  (its directory in the workspace), or by default the current directory.
  They exit with 0 for Accepted, 1 for another verdict, 2 for Compile
  Error and 3 for an error. `--std` raises the C++ standard from C++11
  (or the C standard from C11), and `--sanitize` builds with
  AddressSanitizer and UBSan. Details in
  [docs/workspace.md](docs/workspace.md).
- `--json` prints machine-readable output instead. The format is versioned
  and described in [docs/json.md](docs/json.md), so editor plugins and GUIs
  can build on it.
- `lc help` describes the commands and `lc version` prints the version.

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
your own packs there (see below), and later milestones keep your progress and
submission history there. `make install` would copy the bundled packs over
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

## Repository layout

```
include/lc.h            public API of the core library
src/core/               liblc.a: context and errors, arena, strings, files,
                        pack loader, tests.txt parser, markdown parser, JSON writer,
                        types and signatures, values, comparison, wire format,
                        runner (processes and limits), judge (verdicts and events),
                        languages and standards, code generation, building,
                        the workspace
src/cli/                lc: argument handling, the commands, ANSI output, pager
runtime/                the preludes and harness runtimes built with every solution
tools/embed.awk         turns runtime/ into C arrays at build time
problems/               problem packs, one NNNN-slug directory each
tests/unit/             unit tests, one program per suite
tests/fixtures/         fixture packs used by the tests: misbehaving programs for
                        the runner (run/), solutions for every verdict and type
                        (judge/), signatures for code generation (codegen/), and
                        C++ solutions of the shipped problems (solutions/)
tests/expected/         golden files: the e2e tests' output, generated code
tests/e2e.sh            end-to-end tests of the lc binary
docs/PLAN.md            design, architecture and milestones
docs/workspace.md       solving problems: lc start, test and submit, editors
docs/problem-format.md  the problem pack format, types and test literals
docs/wire.md            the wire format between lc and a solution's harness
docs/judging.md         building, running, the limits, and the verdict rules
docs/json.md            the --json output format
```
