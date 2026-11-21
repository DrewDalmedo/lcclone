# Writing problem packs

This is for people who write problem packs: the files of a pack, how
`gen.c` makes random cases, how `lc check` checks a pack, and
`MANIFEST.tsv`, the list of problems still to write. The format of
`problem.md` and `tests.txt` themselves is in
[problem-format.md](problem-format.md).

**Everything in a pack is our own.** A pack keeps LeetCode's number, slug,
title and function signature, so that solutions move between the two, but
the statement, the examples and every test case are written from scratch.
LeetCode's text and test data are copyrighted and its terms forbid
scraping, so nothing is copied from it.

## The workflow

1. **Write `problem.md`**: the header with the signature, and the
   statement in your own words, with your own examples and the
   constraints.
2. **Write `ref.c`** (or `ref.cpp`), the reference solution. Add
   **`brute.c`**, a slow solution that is obviously right, whenever one is
   cheap to write, and **`gen.c`**, which makes random cases.
3. **Write `tests.txt`**: the examples (`@example`), hand-written edge
   cases (an empty input, a single element, extreme values, duplicates),
   random cases (`@gen`), and a few full-size cases (`@full`), at the
   limits of the constraints, so that a slow solution runs out of time
   (see [Full-size cases](#full-size-cases)). Leave out the expected
   results (`=> ...`) if you like.
4. **Run `lc check --regen`**: it makes the `@gen` cases with `gen.c`,
   writes the hashes of the `@full` ones, and works out every expected
   result with `ref`.
5. **Run `lc check`**, and fix what it finds: it builds the stubs, runs
   `ref` on every case, and compares `ref` with `brute` on random small
   cases.
6. **Update `MANIFEST.tsv`**: the problem's status goes from `todo` to
   `draft`, and to `verified` once the pack has been read over.

```sh
export LC_PROBLEMS=$PWD/problems
lc check --regen 121        # rewrite tests.txt
lc check 121                # check the pack
lc check --quick            # check every pack, as CI does on each change
```

## The files of a pack

```
problems/0121-best-time-to-buy-and-sell-stock/
  problem.md    header and statement
  tests.txt     the cases
  ref.c         the reference solution (or ref.cpp)
  brute.c       optional: slow and obviously right (or brute.cpp)
  gen.c         optional: makes random cases and the full-size ones
  slow.c        optional: right, but too slow at full size (or slow.cpp)
```

**`ref`, `brute`, `slow`** are written like any solution: just the
function, or the `Solution` class in C++, with no `#include`s. Everything
shipped must build as C11 or C++11, the defaults, so that it builds
whatever standard a user picks. A pack has `ref.c` or `ref.cpp`, not
both; the same goes for `brute` and `slow`. `slow` is a solution that is
right but too slow, such as the `O(n^2)` method a problem's constraints
are there to turn away: `lc check` makes sure it runs out of time on the
full-size cases ([Time limits](#time-limits)).

**`gen.c`** is always C. See below.

## gen.c

`gen.c` defines one function, which prints one case:

```c
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100000, i;
    int top = small ? 20 : 10000;
    int *prices = malloc((size_t)n * sizeof *prices);

    for (i = 0; i < n; i++)
        prices[i] = (int)rnd(0, top);
    put_ints(prices, n);
    free(prices);
}
```

It prints one line per input, in the order of the signature's parameters
(then the `hidden:` ones), each a literal as in `tests.txt`. `small` asks
for a case small enough for `brute` to work out quickly; otherwise make a
full-size one, up to the problem's constraints. Every case `gen` makes
must keep to the constraints, since `ref` may rely on them. Full-size
cases are what catch a solution that is right but too slow, so make some
of them slow for a naive method too: no duplicate to find early, or the
answer at the far end. The seed can pick the shape.

`lc` builds `gen.c` with `runtime/gen/lc_gen.h` in front of it (so it
needs no `#include`s: the usual C headers are there) and a `main()` after
it. The random numbers start afresh from each case's seed, so **a seed
always makes the same case**, on every system. Use only these functions
for randomness, never `rand()` or the time, and never two of them in one
expression whose order C leaves open, such as `a[rnd(0, n - 1)] =
rnd_char("ab")` or `f(rnd(1, 9), rnd(1, 9))`: gcc and clang call them in
different orders, so the case would depend on the compiler.

| Function | What it does |
|----------|--------------|
| `long long rnd(long long lo, long long hi)` | a whole number from `lo` to `hi`, both included, each as likely |
| `double rnd_double(double lo, double hi)` | a number from `lo` up to `hi` |
| `int rnd_chance(int percent)` | 1 with a chance of `percent` in 100 |
| `char rnd_char(const char *chars)` | one of the characters of `chars` |
| `void rnd_shuffle(int *a, int n)` | puts `a[0..n)` in a random order |
| `uint64_t rnd_u64(void)` | 64 random bits |
| `unsigned long long gen_seed(void)` | the seed of the case being made |

| Function | Prints one input line |
|----------|-----------------------|
| `put_int(long long v)` | `-5` |
| `put_double(double v)` | `2.5`, the shortest text that reads back as `v` |
| `put_bool(int v)` | `true` or `false` |
| `put_char(char c)` | `"a"` |
| `put_string(const char *s)` | `"a b"`, with `"`, `\` and control characters escaped |
| `put_ints(const int *a, int n)` | `[1,2,3]`; also `put_longs`, `put_doubles`, `put_bools` |
| `put_chars(const char *a, int n)` | `["a","b"]` (a `char[]`) |
| `put_strings(char *const *a, int n)` | `["ab","c"]` |
| `put_int_grid(const int *a, int rows, int cols)` | `[[1,2],[3,4]]`, from `a` row by row; also `put_char_grid` |
| `put_raw(const char *literal)` | any literal, as written |

The program `lc` builds is run as `gen SEED small|large [COUNT]`, and
prints COUNT cases for the seeds SEED, SEED+1, ..., with a blank line
between them. So `gen()` must not print blank lines of its own.

## Generated cases

A case of `tests.txt` marked `@gen SEED` (full size) or `@gen SEED small`
is made by `gen.c` from that seed:

```
@gen 1 small
[15,7,5,17,20,20,10]
=> 15

@gen 2
[2020,5058,2786, ...]
=> 10000
```

Write just the `@gen` line, and `lc check --regen` fills in the case
under it. `lc check` makes the case again and fails when it is no longer
what `gen.c` makes (after `gen.c` changed), so the file and the generator
can't drift apart. To `lc test` and `lc submit` it is a case like any
other.

Full-size cases can be large: 10^5 numbers are half a megabyte to a
megabyte of `tests.txt`. So a pack stores them as `@gen` cases only when
its constraints are small; the big ones are `@full` cases, below.

## Full-size cases

A case at the limits of the problem's constraints is what tells a
solution that is right but too slow, or that overflows only on big
numbers, from a good one, as on LeetCode. Such a case can be megabytes,
so `tests.txt` keeps only a line for it, and `gen.c` makes it again when
a solution is judged (`docs/PLAN.md` §6.4):

```
@full 101 3f2a9c1e
=> 99999

@full 102 77c0d2e1
```

- **The seed** is given to `gen(0)`, as for `@gen 101`: the same seed
  makes the same full-size case either way.
- **The hash** is of the input `gen.c` makes. When the input made at
  judging time has another hash, the case is not judged: `lc` stops with
  an error that names the seed, which says that the generator makes
  another case on this system (see the rules below). `lc check` makes
  each case and compares the hashes too, on every system CI runs on.
- **The expected result** is kept when it is short (100 bytes at most:
  a number, a bool, a short list). A longer one, such as a sorted array,
  isn't: the reference solution works it out when the case is judged,
  and the result is kept in the cache, `$XDG_CACHE_HOME/lcclone/full/`,
  under the build keys of `gen` and `ref` and the seed.
- **Writing them:** add `@full SEED` lines, and `lc check --regen` adds
  the hashes and the short results. It writes them again when `gen.c`
  changes: like `@gen` cases, they can't drift apart from the generator,
  since `lc check` fails when one is stale.
- **Where:** after the stored cases, by custom, so that a submission
  meets them last, as on LeetCode. Two or three are usually enough: one
  of the shapes that are slowest for a naive method, and a random one.
- **Judging:** `lc submit` runs them with the other cases; `lc test`
  doesn't, but runs those in your `cases.txt` (`@full SEED`, with or
  without the hash). Building `gen.c` takes `cc`.

**What `gen.c` must do for them.** It runs on every machine that judges
the case, so it must make a full-size case in well under the time limit:
`lc check` warns when it takes more than a third of it. And it must make
the same case everywhere:

- use only `rnd()` and its friends for randomness (above);
- call no `libm` function but `sqrt` (`exp`, `log`, `pow`, `sin` and the
  rest round differently from one C library to another); `fabs`,
  `floor`, `ceil` and `fmod` are exact, and fine;
- sort nothing whose order among equal items matters: `qsort` isn't
  stable, and orders ties differently on each system;
- `lc` builds `gen.c` with `-ffp-contract=off`, so that the compiler
  can't fuse a multiply and an add into one instruction, as clang does on
  arm64, which rounds once instead of twice.

**Up to the edges, never past them.** Full-size cases reach the limits
the statement allows: the largest sizes, values at their limits, and an
answer near the `int` limit when the statement says it fits in one. A
case past them fails right solutions, and neither `brute` nor the
sanitizers notice, so read `gen.c` against every guarantee of the
statement, not only the sizes.

**Worst cases, not only big ones.** Random input is often easy for a slow
method: a duplicate turns up early, or the answer sits near the start.
Use the seed to pick among shapes (`gen_seed() % 3`), and make some of
them the slowest for the naive method.

## Time limits

A pack's `time_limit_ms` should reject what LeetCode rejects, such as an
`O(n^2)` method at n = 10^5, and accept what it accepts, such as `O(n^2)`
at n = 10^4; the default of a second can't do that for every problem.
On the full-size cases, `lc check` measures what the limit is set from:

- **`ref`** must take at most a third of the limit on every case, as on
  the others: the limit leaves room for slower machines and for other
  right solutions.
- **`brute`** runs on them under ten times the limit, and `lc check`
  reports its slowest time as a multiple of the limit (a note). It warns
  when `brute` takes between a third of the limit and three times it on a
  case: there the verdict on a method that slow would depend on the
  machine and the compiler. When `brute` finishes, its results must agree
  with `ref`'s, so `ref` is checked at full size too.
- **`slow`** (`slow.c` or `slow.cpp`), if the pack has one, must run out
  of time on at least one full-size case, and be right where it doesn't.
  Since `lc check` runs on every CI system, a faster machine or compiler
  that lets it pass is noticed.
- **`ref` with the sanitizers** runs them too, where the compiler has
  sanitizers (not musl's gcc: a note says so), so that UBSan catches a
  signed overflow that only the big numbers reach. It doesn't catch a
  `long long` cut down to an `int`; read `ref`'s types.

These checks run `brute` for up to ten times the limit per case, so they
are slow. `lc check --quick` leaves them out; it still makes every
full-size case and runs `ref` on it. CI runs them only on the packs a
change touches ([CI](#ci)).

## lc check --regen

`lc check --regen` rewrites `tests.txt` before checking the pack:

- the case under each `@gen` line is made again with `gen.c`;
- each `@full` line gets the hash of the case `gen.c` makes for it;
- every case gets its expected result from `ref`. A result that is
  already there and matches `ref`'s (as the problem's `compare:` mode
  judges it, so `[1,0]` stays for an unordered result) is kept as it was
  written. A full-size case's result is written only when it is at most
  100 bytes long; a longer one's `=>` line is removed;
- everything else, comments and blank lines included, stays as it is.

While `--regen` reads it, a case may leave out its `=> ...` line (it then
ends at a blank line), or have nothing after the `=>`, an `@gen` line
may have no case under it yet, and an `@full` line no hash. If `ref` fails on a case (a crash, or too
slow), nothing is written: the case most likely breaks the constraints.

## lc check

`lc check` checks packs: the ones named (pack directories, IDs or slugs),
or every pack of the problem directory. For each pack:

1. `problem.md` loads, and its ID, slug, title and difficulty agree with
   its row in `MANIFEST.tsv`, when the problem directory has one;
2. `tests.txt` reads, every case fits the signature, and some case is an
   `@example`;
3. the C and C++ stubs (what `lc start` writes) build with `lc`'s
   harness, for each compiler that is installed;
4. every `@full` case is what `gen.c` makes (its hash), and `gen.c` makes
   it in at most a third of the time limit (a warning otherwise);
5. `ref` builds and passes every case, full-size ones included, and its
   slowest case takes at most a third of the time limit (a warning
   otherwise: the limit should leave room for slower machines and other
   correct solutions);
6. every `@gen` case is what `gen.c` makes;
7. `ref` and `brute` give the same results on 300 small random cases from
   `gen.c` (`--count` and `--seed` change that), as the problem's
   `compare:` mode judges them: under `unordered 0` they may list a result
   in different orders;
8. unless `--quick`, on the full-size cases: `brute`'s time and results,
   `slow`, and `ref` with the sanitizers ([Time limits](#time-limits)).

A problem that needs something `lc` can't judge yet (a binary tree, say)
has only its `problem.md` and `tests.txt` checked; it counts as "not
judged yet", not as a failure. A function that returns `void` and has no
`check:` (see [problem-format.md](problem-format.md#check)) is different:
it can never be judged as it is, so `lc check` reports an error at its
`signature:` line. When `ref` or `brute` fails a case, a reason the
harness gave (such as a count that doesn't fit the array) is in the
message.

What `lc check` finds is printed as `file:line: error: message` lines, so
editors can jump to it, with the input and the outputs below a failed
case:

```
problems/0121-best-time-to-buy-and-sell-stock/ref.c: error: ref.c and brute.c disagree on the random case for seed 80 (small)
    Input:    prices = [7,16,13,3,10,1,12,13]
    ref.c:    10
    brute.c:  12
0121-best-time-to-buy-and-sell-stock: FAILED (1 error)
```

A failed random case is easy to look at again: the seed makes it, and
`--seed` starts there. The exit status is 0 when every pack is fine, 1
when one isn't (or `MANIFEST.tsv` is broken), and 3 for a usage error.
`--json` prints JSON Lines ([json.md](json.md#lc-check---json)).

Each random case runs `ref` and `brute` once each, a process apiece, so
300 cases take a few seconds. `make test` checks the shipped packs with
`--quick` and 50; `make check-packs` with the default 300 (`PACKS='1 121'`
checks only those, and `CHECKFLAGS` adds options such as `--quick`).

## CI

CI checks every pack on each push and PR, leaving out the slow checks on
the full-size cases (`--quick`): in `make test` on every system, and with
`make check-packs CHECKFLAGS=--quick` on Linux (gcc and clang) and macOS
too. Over every pack those checks would take hours, so they run, on those
systems and on Alpine (musl), only for

- the packs the push or PR changes: those with a file under
  `problems/NNNN-slug/` that differs from the base. For a PR that is its
  base branch; for a push, the commit before it, or for a new branch,
  where it leaves `main`;
- every pack, when a file under `runtime/` changes (`lc_gen.h`
  included), on Mondays, and when the workflow is run by hand
  (`gh workflow run CI --ref BRANCH`). Run it by hand for a change to how
  `lc` builds, runs or times programs that is outside `runtime/`.

`tools/full-packs.sh BASE` prints the packs CI picks, and the first job,
"Packs to check in full", shows them.

## MANIFEST.tsv

`problems/MANIFEST.tsv` lists the LeetCode problems in scope, with how far
each one's pack is. It holds only facts: numbers, slugs, titles and
difficulties. It lists every Easy problem (see
[How MANIFEST.tsv was built](#how-manifesttsv-was-built)); medium and
hard ones come with M9. One row per problem, in increasing ID order, with seven
fields separated by tabs:

```
# id	slug	title	difficulty	premium	group	status
1	two-sum	Two Sum	easy	no	unordered	verified
104	maximum-depth-of-binary-tree	Maximum Depth of Binary Tree	easy	no	tree	draft
175	combine-two-tables	Combine Two Tables	easy	no	skip:sql	todo
```

| Field        | Value |
|--------------|-------|
| `id`         | the LeetCode number |
| `slug`, `title` | LeetCode's |
| `difficulty` | `easy`, `medium` or `hard` |
| `premium`    | `yes` for a premium-only problem, else `no` |
| `group`      | the harness feature the problem needs (below), or `skip:REASON` when it is out of scope, e.g. `skip:sql`, `skip:premium`, `skip:javascript` |
| `status`     | `todo` (no pack yet), `draft` (a pack, not yet read over) or `verified` (passes `lc check` and was read over) |

The groups, from `docs/PLAN.md` §2.2:

| Group          | Problems |
|----------------|----------|
| `plain`        | plain functions of scalars (`char` included), arrays, strings and 2D arrays |
| `bits`         | `uint32` and `long` |
| `float`        | `double` results, compared with a tolerance |
| `inplace`      | a parameter changed in place (`check:`) |
| `unordered`    | results in any order (`compare: unordered`) |
| `list`         | `ListNode` |
| `list-builder` | lists built specially: cycles, intersections (`hidden:`) |
| `tree`         | `TreeNode` |
| `tree-ref`     | an argument that refers to a node of another |
| `nary`         | `NaryNode` |
| `design`       | a class and a sequence of method calls |
| `api`          | functions the harness provides (`api.c`) |
| `checker`      | many valid answers (`checker.c`) |
| `extend`       | anything else (`extend.c`) |

Lines starting with `#` and blank lines are ignored. `lc check` checks
every pack against its row, and fails for a pack with no row; a broken
`MANIFEST.tsv` fails the check as a whole.

`lc stats --manifest` reports how far the problem set is: the problems by
status and by group, the ones out of scope, and three kinds of loose
ends: rows marked `draft` or `verified` that have no pack, rows still
`todo` that have one, and packs that aren't listed.

## How MANIFEST.tsv was built

The Easy rows were made once, on 2026-10-30, from a public problem index
rather than from LeetCode itself, so no request went to LeetCode. Groups
were assigned by script from each problem's function signature, then
checked by hand.

**Source and credit.** The facts come from the problem index of
[doocs/leetcode](https://github.com/doocs/leetcode), by the doocs
contributors: `solution/result.json` at commit `1cc0ae26dd63`. That
project is licensed under
[CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/), so
`MANIFEST.tsv` says so in its header, credits it, says what was changed
(only the Easy problems; only number, slug, title, difficulty and premium
flag; the group and status columns are ours) and is shared under the same
license. Keep that header when you edit the file.

**What was used, and what wasn't.** The index has every problem from 1 to
4096, with no gaps; 974 are Easy. From each, only the facts above were
kept. Its C++ and C code templates were read to pick a group. Its problem
statements were only searched for a few phrases ("any order", "return
any", "in-place", "first k"), as hints for the group: no text was copied.

**Out of scope.** A problem in a category lc doesn't cover gets
`skip:<category>` (`skip:sql`, `skip:pandas`, `skip:javascript`,
`skip:shell`, `skip:concurrency`), even when it is also premium; its
premium column still says `yes`. A premium problem of the algorithms
category gets `skip:premium`. Of the 974 rows, 223 are out of scope.

**Groups.** From the C++ template, in this order:

1. a class other than `Solution` (or a `Solution` constructor): `design`;
2. an interactive problem, or one whose template declares an API it
   calls: `api`;
3. `Node*` with `children`: `nary`; `ListNode`: `list` (141 and 160:
   `list-builder`); `TreeNode`: `tree` (1379: `tree-ref`);
4. a type outside lc's type table: `extend` (no Easy problem needed it);
5. otherwise, from the statement's wording and the return type: several
   answers accepted, `checker`; "in any order", `unordered`; a `void`
   result or a "first k" result, `inplace`; a `double` result, `float`;
   `uint32_t` or `long long` anywhere, `bits`; else `plain`. A `char`
   argument stays `plain`, as in the shipped 744.

A problem that needs two features gets the group of the later milestone
(M8's `design`, `api` and `checker` before M7's lists and trees, before
M6's groups). So 108 is `checker`, and 257, 501 and 637 are `tree`.

**Checked by hand.** 37 first guesses were corrected: 26, 27 and
1089 to `inplace`; 108, 888, 905, 922, 942, 1030, 1304, 1317, 1374, 1417,
1576, 2099, 2733, 2900, 2903, 3692, 3963, 3992 and 4074 to `checker`; 448
to `unordered`; and 14 rows the wording matched by accident back to
`plain` (205, 541, 1295, 1403, 1640, 1816, 1961, 2138, 2239, 2243, 2500,
3541, 3794, 3875). The rows of the shipped packs were kept as they were.

**Worth a second look when the pack is written:**

- 448 is `unordered` because `docs/PLAN.md` §2.2 says so, though its
  statement doesn't mention order;
- 1030 (ties in any order within a sort) and 3692 (a string whose
  characters may come in any order) are `checker`, since `unordered`
  can't express them;
- 2099, 2733, 2903 and 4074 are `checker` because several answers are
  accepted;
- 27 is `inplace`, but its first k elements may come in any order;
- 191 and 2235 are `plain`: LeetCode's signatures now use `int`, though
  `docs/PLAN.md` §2.2 gives them as `bits` examples.

**Differences from LeetCode today.** LeetCode now gives 190 as
`int reverseBits(int n)`; the shipped pack keeps `uint32`. The title of
1560 has a double space on LeetCode, kept as it is, so its pack's title
must have it too (`lc check` compares them).

The counts, by group: `plain` 639, `tree` 31, `checker` 19, `unordered`
16, `design` 10, `bits` 8, `float` 7, `list` 7, `inplace` 6, `nary` 3,
`list-builder` 2, `api` 2, `tree-ref` 1; and out of scope: `skip:sql` 113,
`skip:premium` 61, `skip:javascript` 30, `skip:pandas` 15, `skip:shell` 2,
`skip:concurrency` 2. `lc stats --manifest` gives the current ones.
