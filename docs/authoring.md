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
   random cases (`@gen`), and a few cases at the limits of the
   constraints, so that a slow solution runs out of time. Leave out the
   expected results (`=> ...`) if you like.
4. **Run `lc check --regen`**: it makes the `@gen` cases with `gen.c` and
   works out every expected result with `ref`.
5. **Run `lc check`**, and fix what it finds: it builds the stubs, runs
   `ref` on every case, and compares `ref` with `brute` on random small
   cases.
6. **Update `MANIFEST.tsv`**: the problem's status goes from `todo` to
   `draft`, and to `verified` once the pack has been read over.

```sh
export LC_PROBLEMS=$PWD/problems
lc check --regen 121        # rewrite tests.txt
lc check 121                # check the pack
lc check                    # check every pack, as CI does
```

## The files of a pack

```
problems/0121-best-time-to-buy-and-sell-stock/
  problem.md    header and statement
  tests.txt     the cases
  ref.c         the reference solution (or ref.cpp)
  brute.c       optional: slow and obviously right (or brute.cpp)
  gen.c         optional: makes random cases
```

**`ref`, `brute`** are written like any solution: just the function, or
the `Solution` class in C++, with no `#include`s. Everything shipped must
build as C11 or C++11, the defaults, so that it builds whatever standard a
user picks. A pack has `ref.c` or `ref.cpp`, not both; the same goes for
`brute`.

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
must keep to the constraints, since `ref` may rely on them.

`lc` builds `gen.c` with `runtime/gen/lc_gen.h` in front of it (so it
needs no `#include`s: the usual C headers are there) and a `main()` after
it. The random numbers start afresh from each case's seed, so **a seed
always makes the same case**, on every system. Use only these functions
for randomness, never `rand()` or the time.

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

Full-size cases can be large: 10^5 numbers are about half a megabyte of
`tests.txt`.

## lc check --regen

`lc check --regen` rewrites `tests.txt` before checking the pack:

- the case under each `@gen` line is made again with `gen.c`;
- every case gets its expected result from `ref`. A result that is
  already there and matches `ref`'s (as the problem's `compare:` mode
  judges it, so `[1,0]` stays for an unordered result) is kept as it was
  written;
- everything else, comments and blank lines included, stays as it is.

While `--regen` reads it, a case may leave out its `=> ...` line (it then
ends at a blank line), or have nothing after the `=>`, and an `@gen` line
may have no case under it yet. If `ref` fails on a case (a crash, or too
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
4. `ref` builds and passes every case, and its slowest case takes at most
   a third of the time limit (a warning otherwise: the limit should leave
   room for slower machines and other correct solutions);
5. every `@gen` case is what `gen.c` makes;
6. `ref` and `brute` give the same results on 300 small random cases from
   `gen.c` (`--count` and `--seed` change that).

A problem that needs something `lc` can't judge yet (a binary tree, say)
has only its `problem.md` and `tests.txt` checked; it counts as "not
judged yet", not as a failure.

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
50; `make check-packs`, which CI runs, with the default 300.

## MANIFEST.tsv

`problems/MANIFEST.tsv` lists the LeetCode problems in scope, with how far
each one's pack is. It holds only facts: numbers, slugs, titles and
difficulties. One row per problem, in increasing ID order, with seven
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
| `plain`        | plain functions of scalars, arrays, strings and 2D arrays |
| `bits`         | `uint32`, `long` and `char` |
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
