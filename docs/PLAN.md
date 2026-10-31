# lcclone: plan

An offline LeetCode-style practice tool for Unix. You browse problems, write
solutions in **C or C++** in your own editor, run them against local tests, get
LeetCode-style verdicts, and track progress. There is no network and no
account.

It ships as a command-line tool first. The core is a library with no UI code in
it, so a GUI can be built on top later without rewriting anything (§4).

## 1. Goals and constraints

- **Tool language:** C11 plus POSIX.1-2008 (`_POSIX_C_SOURCE 200809L`). Only
  libc. No ncurses, no JSON, sqlite or regex libraries.
- **Solution languages:** C and C++. Users need `cc` for C solutions and `c++`
  for C++ solutions. A language whose compiler isn't installed is simply
  hidden.
- **Language standards:** solutions compile as **C++11** by default, and you
  can raise that to C++14/17/20/23 (§5.6). C solutions compile as C11.
- **Editor:** designed around **Neovim** (§3.1). Any `$EDITOR` still works.
- **Build:** a plain POSIX `Makefile` producing `liblc.a` (the core) and `lc`
  (the CLI).
- **Platforms:** Linux (glibc and musl), macOS, FreeBSD and OpenBSD.

Not in v1:
- network sync
- a GUI (but one is planned for)
- languages other than C and C++
- contests
- a sandbox against hostile code

You run your own code, so resource limits only guard against accidents like
infinite loops and memory blowups. They are not a defense against attackers.

## 2. Problem scope: every free easy problem first

The first problem set is **every non-premium Easy problem on LeetCode that can
be solved in C or C++**. Medium and hard come after that.

**Out of scope:**
- premium-only problems;
- the Database (SQL), Pandas, JavaScript, Bash and Concurrency categories.

My rough estimate is 500–600 problems in scope. The exact list and count come
from the manifest (§6).

### 2.1 Content rules

- **Kept, so you can cross-reference:**
  - each problem's LeetCode number, slug and title. **The problem ID is always
    the LeetCode number:** `lc show 1` is Two Sum, and the pack directory is
    `problems/0001-two-sum/`. `lc check` checks that the ID and slug match the
    manifest;
  - its function signature (names and types), so a solution moves between
    lcclone and LeetCode unchanged.
- **Written by us:** every problem statement, example and test case.
  LeetCode's text and test data are copyrighted and their terms forbid
  scraping, so none of it is copied.
- **C signatures where LeetCode has none:** some problems don't offer C on
  LeetCode. For those we write a C signature using the same conventions.

### 2.2 What the easy problems need from the harness

The free easy problems sort into the groups below. Each group needs a harness
feature, and the milestones (§9) bring those features in roughly this order.

| Group | Examples (LeetCode #) | Needs |
|-------|-----------------------|-------|
| Plain functions (the large majority) | 1, 9, 13, 14, 20, 70, 121, 125, 136, 169 | scalars, arrays, strings, 2D arrays |
| Bits and odd scalars | 190, 191, 2235 | `uint32`, `long`, `char` |
| Floating-point answers | 643, 1491 | `double` compared with a tolerance |
| In-place edits | 26, 27, 88, 283, 344, 1089 | checking a parameter after the call, or the first k elements |
| Linked lists | 21, 83, 203, 206, 234, 876, 1290 | `ListNode` |
| Linked lists built specially | 141 (cycle at `pos`), 160 (two lists that join) | builders, hidden inputs |
| Binary trees | 94, 100, 101, 104, 110, 111, 112, 226, 543, 617, 700 | `TreeNode` |
| Tree node references | 1379 | an argument that refers to a node inside another argument |
| N-ary trees | 559, 589, 590 | `NaryNode` |
| Design (class) problems | 225, 232, 303, 703, 705, 706, 933, 1603 | a class plus a sequence of method calls |
| Provided API | 278 (`isBadVersion`), 374 (`guess`) | harness-supplied functions, hidden inputs |
| Many valid answers | 108, 905, 942 | a custom checker |
| "Any order" answers | 1, 349, 448, 2215 | unordered comparison, chosen per nesting level |

## 3. Command-line interface

```
$ lc list --difficulty easy --tag tree --status todo
$ lc show 104                  # statement rendered in the terminal, via $PAGER
$ lc start 104 --lang cpp      # creates ~/lc/0104-maximum-depth-of-binary-tree/, opens $EDITOR
$ lc test                      # inside a problem directory: example cases plus your cases.txt
$ lc test ~/lc/0104-*/solution.cpp   # or name the solution file (this is what editors do)
$ lc submit 104                # every case, including hidden ones; records the result
$ lc stats                     # solved counts by difficulty and tag
$ lc random --difficulty easy --status todo
$ lc check problems/0104-*     # validates a problem pack (for authors)
```

**Naming a problem.** You can give a problem as its number (`104`), its slug or
a solution file path. Or leave it out, and `lc` uses the problem directory
you're in.

**Expected output for your own cases.** For your cases in `cases.txt`, or a
file given with `--case FILE`, the expected output comes from running the
reference solution, unless the case gives it with `=>`.

**Machine-readable output.** Every command takes `--json`. The output schema is
versioned (`"schema": 1`) and kept stable. This lets a GUI, or an editor
plugin, be written in any language by running `lc` as a subprocess.
- **Streaming:** `test` and `submit` stream JSON Lines, one object per event
  as it happens: `build_start`, `build_done` (with parsed compiler
  diagnostics), `case_done` and `done`. A front-end can show live progress.
- **No parsing in `lc`:** writing JSON needs only a ~150-line writer; nothing
  in `lc` ever has to parse JSON.

**Verdicts:** Accepted, Wrong Answer, Compile Error, Runtime Error, Time Limit
Exceeded, Memory Limit Exceeded and Output Limit Exceeded.

When a case fails, `lc` shows:
- the case;
- the input, the expected result and your result;
- anything your code printed;
- for a runtime error, the signal or exit code and the tail of stderr.

**Exit codes:**
- 0: Accepted;
- 1: any other verdict;
- 2: compile error;
- 3: a usage or internal error;
- 130: ^C stopped the run.

### 3.1 Working from Neovim

Neovim is the editor this is designed around. Support comes in three layers.
Each one works without the next.

**Workspace layout.** `lc start` creates one directory per problem:

```
~/lc/                                  (workspace = in config; keep it in git if you like)
  .clangd                              generated: tells clangd about the preludes and standards
  0104-maximum-depth-of-binary-tree/
    README.md                          the statement in markdown (regenerated by lc)
    solution.cpp                       your code; never overwritten once it exists
    solution.c                         only if you start it in C as well
    cases.txt                          your own test inputs; `lc test` runs them too
```

**Opening the editor.**
- **Which editor:** `lc start` runs `$VISUAL`, then `$EDITOR`, then `vi`,
  through `sh -c`, so a value that includes arguments works.
- **Showing the statement too:** set `open_statement = yes` in the config, and
  the statement is passed as an extra file. With `editor = nvim -O`, the
  statement and your solution then open side by side.
- **Without an editor:** `--no-edit` creates the files and prints the path.
  The plugin uses this.

**Layer 1: clangd works out of the box.**
- **The problem:** solution files have no `#include`s, so clangd would flag
  every `vector` and `ListNode` as an error.
- **The fix:** `lc` writes `~/lc/.clangd`, which adds the right prelude and
  `-std=` for `.c` and `.cpp` files separately. It rewrites the file whenever
  `cstd` or `cxxstd` changes. (As built in M3: the preludes it points at are
  copies `lc start` keeps in `~/lc/.lcclone/`, so clangd works from a
  source tree too, and a `.clangd` that `lc` didn't write is left alone.)
- **The result:** completion, diagnostics and go-to-definition work in
  solution files with Neovim's built-in LSP client alone, no plugin needed.

```yaml
If:
  PathMatch: .*\.c
CompileFlags:
  Add: [-xc, -std=c11, -include, "/home/me/lc/.lcclone/lc_prelude.h"]
---
If:
  PathMatch: .*\.(cpp|cc|cxx)
CompileFlags:
  Add: [-xc++, -std=c++11, -include, "/home/me/lc/.lcclone/lc_prelude.hpp"]
```

**Layer 2: plain `:make`.** This works in Neovim (and Vim) without the plugin:
`:set makeprg=lc\ test\ %`.
- **Compile errors** use the standard `file:line:col: error: message` form and
  point at your file (thanks to `#line`), so the default `errorformat` fills
  the quickfix list.
- **Failed cases** are also printed in that form, for example
  `solution.cpp:1: Wrong Answer on case 3 (...)`, so they show up in quickfix
  too.

**Layer 3: a Neovim plugin (`editors/nvim/`).** This is a small Lua plugin in
this repo.
- **Requirements:** Neovim 0.10+ and nothing else.
- **Pickers:** uses Telescope or fzf-lua if one is installed, and
  `vim.ui.select` otherwise.
- **How it works:** it runs `lc --json` through `vim.system()` and never
  touches the core directly. That also makes it the first real front-end built
  on the JSON interface, which proves the GUI path (§4).

| Command                      | What it does                                                    |
|------------------------------|-----------------------------------------------------------------|
| `:Lc list [filters]`         | picker over problems; opens the one you choose                  |
| `:Lc show [id]`              | statement in a split (markdown filetype)                        |
| `:Lc start [id] [c\|cpp]`    | creates the files and opens statement and solution side by side |
| `:Lc test` / `:Lc submit`    | runs in the background for the current buffer, with live progress and the result in a scratch buffer |
| `:Lc random` / `:Lc stats`   | as on the CLI                                                   |
| `:Lc std c++17`              | changes the C++ standard for this Neovim session                |

- **Which problem:** the plugin works it out from the current buffer's path.
- **Compile errors** become `vim.diagnostic` entries on the solution buffer and
  fill the quickfix list. So do sanitizer stack frames that point into your
  file.
- **Failed cases:** the scratch buffer shows the input, the expected result,
  your result and what you printed. One key adds that case to `cases.txt`.
- **Key mappings:** none by default, as is usual for Neovim plugins. The help
  file (`:h lcclone`) suggests some, such as `<leader>lt` and `<leader>ls`.
- **Install:** `make install-nvim` copies the plugin into
  `~/.local/share/nvim/site/pack/lcclone/start/`, which Neovim loads on its
  own. Or point lazy.nvim at `editors/nvim` with `dir =`.

## 4. Architecture: core library plus front-ends

```
          +------------------+      +--------------------------+
          |   lc  (CLI)      |      |  future GUI              |
          |  args, ANSI,     |      |  (links liblc directly,  |
          |  $EDITOR, pager  |      |   or runs `lc --json`)   |
          +--------+---------+      +------------+-------------+
                   |      public API: include/lc.h      |
          +--------v-----------------------------------v-------------+
          |                       liblc (core)                       |
          |  problems | types/values | codegen | build | runner |    |
          |  judge | store (progress, config) | markdown parser      |
          +-----------------------------------------------------------+
```

### 4.1 Rules for the core, so a GUI fits later

- **It never prints, never calls `exit()` and never reads stdin.** Every result
  comes back as structs. Errors come back as return codes plus a message held
  in the context.
- **No global state.** Everything lives in an `lc_ctx`. A GUI can therefore run
  a judge job on a worker thread while the UI thread keeps browsing.
- **No process-wide signal tricks.** That means no `alarm()`, and no handlers
  for `SIGCHLD` or `SIGALRM`:
  - timeouts come from `poll()` timeouts;
  - only the child processes the core started itself are reaped, by PID;
  - the front-end ignores `SIGPIPE` when it starts, and the core treats
    `EPIPE` as an ordinary error.
- **Long operations report progress and can be cancelled:**
  - the caller passes in an event callback and a cancel flag;
  - the runner checks the flag on every pass of its poll loop (at least every
    50 ms) and kills the child process group if it is set.
- **Data, not presentation:**
  - Problem statements are markdown. The core parses them into a list of
    blocks; the CLI turns those into ANSI text and a GUI into its own widgets.
  - The core gives out the solution path and the stub text, and can save a
    solution. Choosing how to edit is the front-end's job: the CLI opens
    `$EDITOR`, while a GUI would embed an editor.

### 4.2 Public API sketch (`include/lc.h`)

```c
typedef struct lc_ctx lc_ctx;

lc_ctx     *lc_open(const lc_options *opt);   /* reads config, indexes problem packs */
void        lc_close(lc_ctx *);
const char *lc_last_error(const lc_ctx *);

size_t            lc_problem_count(lc_ctx *);
const lc_problem *lc_problem_at(lc_ctx *, size_t i);
const lc_problem *lc_problem_find(lc_ctx *, const char *id_or_slug);
/* lc_problem: id, slug, title, difficulty, tags, statement (markdown + parsed blocks), examples */

int lc_languages(lc_ctx *, const lc_lang **out, size_t *n);   /* only installed ones */
int lc_stub(lc_ctx *, const lc_problem *, const lc_lang *, lc_str *out);
int lc_solution_path(lc_ctx *, const lc_problem *, const lc_lang *, lc_str *out);

typedef struct {
    void (*on_event)(void *ud, const lc_event *ev); /* build started/done, case i/n started/done */
    void *ud;
    const volatile sig_atomic_t *cancel;              /* set non-zero to stop */
} lc_hooks;
/* lc_judge_options: std, sanitize, examples / all, your cases, stop_on_failure */
int lc_judge(lc_ctx *, const lc_problem *, const lc_lang *, const char *src_path,
             const lc_judge_options *, const lc_hooks *, lc_report **out);
void lc_report_free(lc_report *);

int lc_progress(lc_ctx *, const lc_problem *, lc_progress_entry *out);
int lc_stats(lc_ctx *, lc_stats_summary *out);
```

**Two ways to build a GUI:**
- a native C GUI (GTK, or something small like raylib or microui) that links
  `liblc` directly;
- a GUI in any language that runs `lc --json`.

Both are possible from day one, and the choice can wait.

**Later option:** a step-by-step judge API. `lc_job_fds()` and `lc_job_step()`
would plug into a GUI's own event loop, so the GUI wouldn't need threads.

## 5. Judge model

### 5.1 Signatures and types

Each problem declares a signature, for example
`int[] twoSum(int[] nums, int target)`. The stubs follow LeetCode's own C and
C++ conventions:

| Spec type  | C parameter                                      | C++ parameter                  |
|------------|--------------------------------------------------|--------------------------------|
| `int`      | `int x`                                          | `int x`                        |
| `long`     | `long long x`                                    | `long long x`                  |
| `uint32`   | `uint32_t n`                                     | `uint32_t n`                   |
| `double`   | `double x`                                       | `double x`                     |
| `bool`     | `bool b`                                         | `bool b`                       |
| `char`     | `char c`                                         | `char c`                       |
| `string`   | `char* s`                                        | `string s`                     |
| `int[]`    | `int* nums, int numsSize`                        | `vector<int>& nums`            |
| `char[]`   | `char* s, int sSize`                             | `vector<char>& s`              |
| `string[]` | `char** strs, int strsSize`                      | `vector<string>& strs`         |
| `int[][]`  | `int** grid, int gridSize, int* gridColSize`     | `vector<vector<int>>& grid`    |
| `char[][]` | `char** board, int boardSize, int* boardColSize` | `vector<vector<char>>& board`  |
| `ListNode` | `struct ListNode* head`                          | `ListNode* head`               |
| `TreeNode` | `struct TreeNode* root`                          | `TreeNode* root`               |
| `NaryNode` | `struct Node* root`                              | `Node* root`                   |

**Return values in C** follow LeetCode's rules:
- an array return adds `int* returnSize`;
- a 2D return also adds `int** returnColumnSizes`;
- returned memory is `malloc`ed by the solution.

**Return values in C++** are plain values, such as `vector<int>`, or pointers.

**Wrappers.** In C++ the function goes inside `class Solution { public: ... };`,
just as on LeetCode.

**C stub:**

```c
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {

}
```

**C++ stub:**

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {

    }
};
```

### 5.2 Special problem kinds

**In-place edits.** `check: param nums` compares a parameter after the call.
`check: prefix nums` checks a returned `k` together with the first `k`
elements of `nums`.

**Builders and hidden inputs.** Some test inputs only shape the data and are
never passed to your function. In 141, `pos` sets where the cycle starts; in
160, `skipA` and `skipB` set where the lists join. The problem declares these
as `hidden:` inputs and names a built-in builder (`cycle`, `intersect`).

**Node references.** For problems like 1379, the declaration
`ref target in cloned` makes the harness look up the matching node by value.

**Design problems.** The tests use LeetCode's two-line format:

```
["MyQueue","push","push","peek","pop","empty"]
[[],[1],[2],[],[],[]]
=> [null,null,null,1,1,false]
```

The problem declares a class and its methods. The harness reads the call
sequence and dispatches each call. The stub follows LeetCode's style for each
language:
- **C:** `typedef struct {...} MyQueue;` with
  - `MyQueue* myQueueCreate();`
  - `void myQueuePush(MyQueue* obj, int x);`
  - ...
  - `void myQueueFree(MyQueue* obj);`
- **C++:** `class MyQueue { public: MyQueue() {} void push(int x) {} ... };`

**Provided APIs.** The problem declares something like
`api: bool isBadVersion(int version)` together with `hidden: int bad`. The
problem pack then supplies `api.c`:
- it is written once in C;
- it is linked into both C and C++ builds (declared `extern "C"` in the C++
  prelude);
- it can also limit how many calls are allowed.

**Escape hatch.** Anything else gets a per-problem `extend.c`, which hooks into
input building and output checking.

### 5.3 Wire format between `lc` and the harness

`lc` parses every LeetCode-syntax literal itself. It converts each input into a
simple wire format of whitespace-separated, length-prefixed tokens and writes
that to the program's stdin. The harness only has to:
1. read the wire format;
2. call your code;
3. write the result back in wire format on **fd 3**.

Your own `printf`/`cout` output stays on stdout and is shown separately.

| Type          | Literal       | Wire                                                     |
|---------------|---------------|----------------------------------------------------------|
| `int`         | `-5`          | `-5`                                                     |
| `bool`        | `true`        | `1`                                                      |
| `string`      | `"a b"`       | `3:a b`                                                  |
| `int[]`       | `[2,7,11]`    | `3 2 7 11`                                               |
| `int[][]`     | `[[1,2],[3]]` | `2 2 1 2 1 3`                                            |
| `string[]`    | `["ab","c"]`  | `2 2:ab 1:c`                                             |
| `TreeNode`    | `[1,null,2]`  | `3 1 n 2` (level order)                                  |
| design calls  | two lines     | `count`, then for each call: method index and arguments |

The two harness runtimes (`lc_rt.c` and `lc_rt.cpp`) each come to only a few
hundred lines.

### 5.4 Checking results

- **`compare: exact`** is the default.
- **`compare: float 1e-5`** compares floating-point values with a tolerance.
- **`compare: unordered 0`, `unordered 1`, `unordered 0,1`** ignore order at
  the nesting levels listed. Problem 2215 needs `1`: order inside each list
  doesn't matter, but the order of the two lists does.
- **`compare: checker`** is for problems with many valid answers. The pack's
  `checker.c` gets the input, the reference output and your output as parsed
  values; it links against the core's value library, so it stays short.

### 5.5 Problem pack format

```
problems/0001-two-sum/
  problem.md    key: value header, then '---', then the statement in markdown
  tests.txt     cases: one line per argument, then '=> expected'; '@example' marks shown cases
  ref.c         reference solution (in C, or ref.cpp)
  gen.c         optional seeded generator for large and random cases
  brute.c       optional slow, obviously-correct solution used to cross-check ref on small cases
  checker.c     optional (compare: checker)
  api.c         optional (provided-API problems)
  extend.c      optional (escape hatch)
```

Example `problem.md`:

```
id: 1
slug: two-sum
title: Two Sum
difficulty: easy
tags: array, hash-table
signature: int[] twoSum(int[] nums, int target)
compare: unordered 0
time_limit_ms: 1000
memory_limit_mb: 256
---
(original statement text, written by us)
```

Both files are line-based and parsed by hand. A value literal is a small JSON
subset (integers, floats, true/false, null, strings with escapes, nested
arrays), so the parser is a recursive-descent parser of about 300 lines.

### 5.6 Building solutions

**One translation unit.** Each build is a single source file made of five
parts:
1. the prelude;
2. `#line 1 "<your file>"`;
3. your source;
4. a `#line` reset;
5. the generated harness.

Compiler errors then point at your file's real line numbers. And because it's
one file, the C++ harness can see `class Solution`.

**Preludes:**
- **C:** stdio, stdlib, string, stdbool, stdint, limits, math, ctype, the node
  structs, and a vendored `uthash.h` (single BSD-licensed header) because
  LeetCode's C environment has it. (M3: `_DEFAULT_SOURCE` is defined first,
  so `strdup` and `M_PI` are there under `-std=c11` on glibc and musl;
  `uthash.h` is not vendored yet.)
- **Node structs, in both preludes, only when the problem uses them:**
  `lc` defines `LC_LISTNODE`, `LC_TREENODE` or `LC_NARYNODE` before the
  prelude. Solutions often define a `struct Node` of their own, which an
  unconditional N-ary `Node` would break, and LeetCode, too, declares them
  only for the problems that need them.
- **C++:** an explicit list of standard headers, `using namespace std;`, and
  the node structs with LeetCode's constructors. It does not use
  `<bits/stdc++.h>`, which only exists in libstdc++ and not in libc++ on macOS
  or the BSDs.
  - **Headers by standard:** newer headers are included only when the active
    standard has them, so raising the standard makes them available just as
    on LeetCode:
    - `__cplusplus >= 201703L`: `<optional>`, `<string_view>`, `<variant>`,
      `<any>`, `<charconv>`;
    - `__cplusplus >= 202002L`: `<bit>`, `<span>`, `<numbers>`, `<ranges>`,
      `<compare>`.

**Compile commands:**
- **C:** `$CC -std=c11 -O2` (and `-lm`)
- **C++:** `$CXX -std=c++11 -O2`

Each runs through `/bin/sh`, as make runs commands, so `CXX='ccache g++'`
works.

**Choosing the C++ standard.** The default is C++11. Ways to raise it, with the
later ones winning:
- `cxxstd = c++17` in the config file (since M4);
- `LC_CXXSTD=c++20` in the environment;
- `--std=c++20` on a single `lc test` or `lc submit` run.

Accepted values are `c++11`, `c++14`, `c++17`, `c++20`, `c++23` and `max`.
`max` means the newest standard the installed compiler accepts.

`lc` checks once which standards the compiler accepts. It compiles an empty
file with each `-std=` flag and caches the results. If you ask for a standard
the compiler doesn't support, you get a clear error rather than a confusing
compile failure.

**Everything shipped is C++11-only,** so it builds under any chosen standard.
That covers the generated harness, `lc_prelude.hpp`, `lc_rt.cpp` and every
`ref.cpp` in the problem packs. For example, they can't use
`std::make_unique`, `auto` lambda parameters or structured bindings.

The C standard can also be changed (`cstd =`, default `c11`).

**Cache.** Build directories are keyed by language *and* standard, so
switching standards never reuses a stale build. (As built in M3: the
runtime is part of the one translation unit rather than an object file of
its own. It is a few hundred lines, so compiling it each time costs little
next to the C++ headers, and `lc` carries it compiled in, so nothing has to
be found on disk. Within a directory, each program is named after a hash of
everything that went into it, compiler included.)

**Sanitizers (`--sanitize`):**
- adds `-g -fsanitize=address,undefined` (and `-fno-sanitize-recover=all`,
  so undefined behaviour stops the program: a Runtime Error);
- for C++, also adds `-D_GLIBCXX_ASSERTIONS` (libstdc++) or the libc++
  hardening flag, so out-of-range `vector` access is caught;
- turns off the `RLIMIT_AS` limit, which conflicts with ASan, and with it the
  memory verdict; triples the time limit; and turns off leak detection,
  since LeetCode-style solutions don't free.

**Cache:** builds go in `$XDG_CACHE_HOME/lcclone/build/<slug>/<lang>-<std>/`,
and a build is redone only when an input changes.

### 5.7 Runner

v1 starts **one process per test case**. That shows exactly which case failed,
and global state can't leak between cases.

1. Create pipes for stdin, stdout, stderr and the fd 3 result, then `fork()`.
2. **In the child:**
   - `setpgid(0,0)`;
   - set `setrlimit` limits:
     - `RLIMIT_CPU` = limit + 1s;
     - `RLIMIT_AS` = 2 × memory limit + 64 MB, a guard against runaway
       programs (skipped under ASan; macOS doesn't enforce it, so there the
       runner watches the program's memory itself);
     - `RLIMIT_STACK` = 256 MB, for deep recursion;
     - `RLIMIT_FSIZE` small;
     - `RLIMIT_CORE` = 0;
   - `chdir` to a scratch directory, `dup2` the pipes, and `execv`.
3. **In the parent:**
   - run one `poll()` loop that writes stdin and reads the other pipes at the
     same time, so big inputs can't deadlock;
   - enforce a wall-clock deadline with `CLOCK_MONOTONIC`;
   - check the cancel flag;
   - cap captured output at about 1 MB, which gives Output Limit Exceeded;
   - when the deadline passes or the job is cancelled, run
     `kill(-pgid, SIGKILL)`.
4. **Collect the result.** `wait4()` gives the exit status, CPU time and peak
   RSS. Memory Limit Exceeded is decided from the peak RSS, not from a
   failed allocation under `RLIMIT_AS`: a crash after a failed `malloc` says
   much less, and the RSS rule works the same on macOS. `ru_maxrss` is in KB
   on Linux and the BSDs but in bytes on macOS, so it
   is normalized. These values map to a verdict.

### 5.8 Storage (XDG)

| What           | Where                                                                   |
|----------------|-------------------------------------------------------------------------|
| Problem packs  | `$LC_PROBLEMS`, then `~/.local/share/lcclone/problems`, then `$(PREFIX)/share/lcclone/problems` |
| Your solutions | `~/lc/<NNNN-slug>/` by default (`workspace =` in config); see §3.1 |
| Progress       | `~/.local/share/lcclone/progress.tsv`: `id status lang best_ms last_ts` |
| History        | `~/.local/share/lcclone/submissions/<id>/<timestamp>-<verdict>.<ext>`   |
| Config         | `~/.config/lcclone/config`, `key = value` (editor, lang, cc, cxx, cstd, cxxstd, cflags, cxxflags, color, workspace) |

Every write is atomic: the data goes to a temp file, then `rename()` moves it
into place. The core re-reads progress before each update, so a CLI and a GUI
running at the same time don't overwrite each other's results.

(As built in M4:
- **Progress:** only `lc submit` records, and `lc test` doesn't. A problem
  is `solved` once accepted and stays so; `attempted` otherwise. `lang` is
  that of the last accepted submission, else of the last one; `best_ms`
  is the slowest case of the best accepted run without `--sanitize`; `-`
  marks an empty field. The update also takes an `fcntl` lock on
  `progress.lock`, and rewrites only its problem's line, so comments,
  unreadable lines and columns of a later version survive.
- **History:** `submissions/NNNN/` (the ID zero-padded like pack
  directories), the time in UTC as `20261030T201503Z`, the verdict as its
  JSON id (`accepted`, `wrong_answer`, ...), and `-2`, `-3` for more in the
  same second.
- **Config:** also `open_statement` (§3.1); `$LC_CONFIG` names another
  file. Which setting wins: a command-line option, then an `LC_`
  variable, then the config, then a general variable (`CC`, `CXX`,
  `VISUAL`, `EDITOR`, `NO_COLOR`), as git puts `core.editor` before
  `$EDITOR`. A bad line is a warning, never fatal. `cflags`/`cxxflags`
  apply to your solutions only.)

## 6. Content pipeline (the bulk of the work)

### 6.1 Manifest

`problems/MANIFEST.tsv` has one row per LeetCode Easy problem:
- number, slug, title;
- premium flag;
- group (from §2.2), or `skip:<reason>`;
- status (`todo`, `draft`, `verified`).

It is built once from the public problem list. That list is only facts:
numbers, titles and difficulty. `lc stats --manifest` reports coverage.

(As built in M4: the columns are `id slug title difficulty premium group
status`, tab-separated in increasing id order; difficulty is there for
when medium and hard come in M9. The groups are named `plain`, `bits`,
`float`, `inplace`, `unordered`, `list`, `list-builder`, `tree`,
`tree-ref`, `nary`, `design`, `api`, `checker` and `extend`. So far it
lists only the shipped problems; filling it in from the public list is
still to do. See docs/authoring.md.)

### 6.2 Authoring each problem

1. **Write the statement.** Use your own words and your own examples, and state
   the constraints.
2. **Write `ref`,** the reference solution. Add `brute` too when the
   straightforward solution is cheap to write.
3. **Write the tests:**
   - hand-written edge cases (empty input, a single element, extreme values,
     duplicates);
   - seeded random cases from `gen.c`;
   - a few cases at the constraint limits, so that slow solutions time out.
4. **Compute the expected outputs.** `lc check --regen` runs `ref` over the
   inputs to produce them.
5. **Cross-check.** `lc check` compares `ref` against `brute` on thousands of
   small random cases, builds the C and C++ stubs, and confirms that `ref`
   passes every case.

(As built in M4: `gen.c` defines `void gen(int small)`, which prints one
case's literals using a portable PRNG and printing helpers from
`runtime/gen/lc_gen.h`; `lc` supplies `main`. Random cases live in
`tests.txt` as ordinary cases under an `@gen SEED [small]` line, so
judging needs no generator; `lc check --regen` makes them and fills in
the expected results, keeping results that already match, and `lc
check` fails when one is stale. The cross-check uses 300 cases by
default (`--count`), since each costs two processes; the batch runner of
§9's later work would make thousands cheap. `lc check` also warns when
`ref` needs more than a third of the time limit.)

### 6.3 Batches

- **Batch by group,** following the order in which harness features land.
  Plain functions start as soon as M3 is done.
- **Size:** about 25 problems per batch. Each batch is one PR, and CI runs
  `lc check` over every pack.

This work parallelizes well across contributors or agents, because every
problem is self-contained.

## 7. Source layout

```
Makefile               POSIX make: liblc.a, lc, `make test`, PREFIX/DESTDIR install
include/lc.h           public core API
src/core/              -> liblc.a
  ctx.c  arena.c  str.c  fs.c       context, errors, memory, strings, files, atomic write
  problem.c  manifest.c             pack loading and index, lookup
  sig.c  types.c                    signature/class/api parsing, type table
  value.c  compare.c  wire.c        literal parse/print, compare modes, wire conversion
  md.c                              markdown subset -> block list
  lang.c  codegen_c.c  codegen_cpp.c  build.c
  runner.c  judge.c                 processes, limits, verdicts, events
  store.c  config.c                 progress, history, XDG paths, config
  json.c                            JSON writer (used by the CLI's --json)
  diag.c                            parses gcc/clang output into file/line/col/severity/message
  workspace.c                       per-problem directories, README.md, .clangd generation
src/cli/               -> lc
  main.c  cmd_*.c  ansi.c  pager.c  editor.c
runtime/
  c/    lc_prelude.h  lc_rt.c  uthash.h
  cpp/  lc_prelude.hpp  lc_rt.cpp
editors/nvim/          Neovim plugin: plugin/lcclone.lua, lua/lcclone/*.lua, doc/lcclone.txt
problems/              MANIFEST.tsv and one directory per problem
tests/
  unit/                core unit tests (a tiny assert macro, no framework)
  fixtures/            misbehaving programs, fixture problems in every group
  e2e.sh               POSIX sh end-to-end verdict checks (C and C++)
```

The core should come to roughly 6–8k lines of C, plus the two runtimes.

## 8. Testing

- **Unit tests:**
  - value parse/print round trips;
  - each compare mode;
  - the wire conversion;
  - the signature, class and api parsers;
  - the markdown parser.
- **Codegen golden tests:** for fixture signatures in every group, the
  generated C and C++ stubs and harnesses are compared against checked-in
  expected files.
- **Runner tests:** fixture programs that each misbehave in one way, each with
  the verdict it should get:

  | Fixture program       | Expected verdict           |
  |-----------------------|----------------------------|
  | spins forever         | Time Limit Exceeded        |
  | segfaults             | Runtime Error              |
  | allocates 2 GB        | Memory Limit Exceeded      |
  | prints 100 MB         | Output Limit Exceeded      |
  | throws in C++         | Runtime Error              |
  | closes stdin early    | no hang or SIGPIPE in `lc` |
  | forks a child         | process group is killed    |

  A further test checks that cancelling a job works.
- **End to end:** each fixture problem gets correct, wrong, slow and crashing
  solutions in C and in C++, and each should get the expected verdict. The C++
  runs happen twice, once with `c++11` and once with `max`.
- **Standard selection:** tests cover config, environment and flag precedence,
  `max`, and the error you get for a standard the compiler doesn't support.
- **Editor support:**
  - the generated `.clangd` is checked with `clangd --check=<file>` on fixture
    C and C++ solutions, which should report no errors;
  - the `file:line` output is checked against Vim's default `errorformat`;
  - the plugin is tested with `nvim --headless` scripts against fixture
    problems. These run in CI on Ubuntu with Neovim installed.
- **CI:** GitHub Actions on Ubuntu (gcc and clang), Alpine (musl) and macOS
  (clang and libc++). Dev builds use `-Wall -Wextra -Wpedantic -Werror`, the
  test build uses ASan and UBSan, and `lc check` runs over all packs.

## 9. Milestones

| #  | Milestone                | Scope                                                                                     |
|----|--------------------------|-------------------------------------------------------------------------------------------|
| M0 | Skeleton                 | `liblc.a` and `lc` build; ctx/errors/arena/str/fs; pack loader; markdown parser; `list` and `show` with `--json` |
| M1 | Values                   | literal parser/printer, the full type table, compare modes, wire format; unit tests      |
| M2 | Runner                   | fork/exec, limits, poll loop, cancel, events, verdicts; misbehaving-program tests         |
| M3 | C and C++ end to end     | codegen for plain functions (scalars, arrays, strings, 2D, `uint32`, `double`, `char`); both runtimes; `start`, `test`, `submit`; editor basics (workspace layout, problem from path or directory, exit codes, `file:line` output, `.clangd`, JSON event stream); 10 problems (1, 9, 14, 20, 70, 121, 190, 643, 744, 867, with reference solutions). **First usable version, already works from Neovim through clangd and `:make`.** |
| M4 | Progress and content tooling | store, `stats`, `random`, history, config; `MANIFEST.tsv`; `lc check` with `--regen` and the brute cross-check |
| M5 | Neovim plugin            | `:Lc` commands, background runs with progress, diagnostics and quickfix, pickers, adding a failed case to `cases.txt`, help file, headless tests |
| M6 | Plain-function easies    | author every problem in the plain-function, bits, float, in-place and any-order groups    |
| M7 | Lists and trees          | `ListNode`, `TreeNode`, `NaryNode`, builders, hidden inputs, node references; author those groups |
| M8 | Design, API and checkers | class/call-sequence harness, `api.c`, `checker.c`, `extend.c`; author those groups. **All free easies covered.** |
| M9 | Medium and hard          | same pipeline, in batches                                                                 |

Feature work and content work overlap. After M4, each new feature milestone
unblocks a group of problems, and those problems can be authored while the next
feature is being built.

Possible later work:
- the GUI;
- a batch mode for the runner;
- a step-by-step judge API for GUI event loops;
- Linux-only hardening (namespaces, seccomp) behind `#ifdef`.

## 10. Risks

- **Content volume.** 500–600 problems, each with original text and strong
  tests, is far more work than the tool itself. The manifest, `gen` + `ref` +
  `brute`, `lc check` and per-group batches are all there to make it routine
  and checkable.
- **Codegen grows with types × 2 languages × problem kinds.** To contain it,
  one type table drives both generators, the wire format stays simple, and
  golden tests catch regressions.
- **Odd problems.** A few problems won't fit any group. `extend.c` keeps them
  from bending the core design.
- **C++ portability.** libstdc++ and libc++ differ: there's no
  `bits/stdc++.h` on libc++, and their hardening flags differ. CI on macOS
  catches these.
- **Newer C++ in solutions written for LeetCode.** Code that uses C++14/17
  features fails under the C++11 default. When a C++ build fails, the error
  message says which standard was used and how to raise it.
- **Timing noise.** Judge on CPU time, and set limits generously (about 3× the
  reference solution's time on the largest cases).

## 11. Decisions so far

- **Solution languages:** C and C++.
- **Interface:** command line first. The core library is kept free of UI code,
  so a GUI can be added later.
- **First problem set:** every free LeetCode Easy problem that can be solved in
  C or C++. Statements and tests are original.
- **C++ standard:** C++11 by default, adjustable through config, environment or
  flag, including `max`. C defaults to C11.
- **Problem IDs:** LeetCode's numbers and slugs.
- **Editor:** Neovim gets first-class support, in three layers: a generated
  `.clangd`, plain `:make`, and a Lua plugin built on `lc --json`. Any
  `$EDITOR` still works.
