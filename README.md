# lcclone

lcclone is an offline, LeetCode-style practice tool for Unix terminals. You
browse problems and read their statements in the terminal, write solutions in
C or C++ in your own editor and, once judging lands, run them against local
tests to get LeetCode-style verdicts. There is no network access and no
account. Problems keep LeetCode's numbers, slugs and function signatures, so a
solution moves between the two unchanged, but every statement, example and
test case in this repository is written from scratch. The tool itself is
C11 plus POSIX.1-2008 and needs nothing beyond libc.

## Status

Milestones **M0**, **M1** and **M2** are in:

- **M0:** the core library (`liblc.a`), the problem pack loader, the
  statement markdown parser, and the `lc list` and `lc show` commands, both
  with `--json` output.
- **M1:** the value layer that judging builds on: typed signatures, test
  literals parsed and checked against them, the comparison modes, and the
  wire format used to talk to a solution's harness.
- **M2:** the runner and the judge. They run a built program once per test
  case under CPU, memory, stack and output limits, and turn what happened
  into a verdict (Accepted, Wrong Answer, Runtime Error, Time, Memory or
  Output Limit Exceeded). They report progress events and can be cancelled.
  See [docs/judging.md](docs/judging.md).

Three problems are included: 1, 20 and 104.

Writing, testing and submitting solutions (`lc start`, `lc test`,
`lc submit`) arrive in M3, which adds building solutions on top of the M2
judge. The design and the milestone table are in
[docs/PLAN.md](docs/PLAN.md) (section 9 lists the milestones).

## Building

You need a C11 compiler and a POSIX `make`, and for `make test` also a C++
compiler (`CXX`, default `c++`) for one test program. The targets are Linux
(glibc and musl), macOS, FreeBSD and OpenBSD; CI builds and tests on Linux
with gcc, clang, musl and bmake, and on macOS.

```sh
make            # builds liblc.a and lc
make test       # unit tests, then the end-to-end tests
```

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

## Commands

```
lc list [--difficulty LIST] [--tag TAG] [--json] [--color=auto|always|never]
lc show <id|slug> [--json] [--no-pager] [--color=auto|always|never]
lc help
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
                        runner (processes and limits), judge (verdicts and events)
src/cli/                lc: argument handling, list and show, ANSI output, pager
problems/               problem packs, one NNNN-slug directory each
tests/unit/             unit tests, one program per suite
tests/fixtures/         fixture packs used by the tests, and programs that
                        misbehave for the runner and judge tests (run/)
tests/e2e.sh            end-to-end tests of the lc binary
docs/PLAN.md            design, architecture and milestones
docs/problem-format.md  the problem pack format, types and test literals
docs/wire.md            the wire format between lc and a solution's harness
docs/judging.md         how programs are run, the limits, and the verdict rules
docs/json.md            the --json output format
```
