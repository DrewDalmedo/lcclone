# Judging

How `lc` builds a solution, runs it and turns what happened into a
LeetCode-style verdict. The runner and the judge landed in milestone M2,
building and the `lc test` and `lc submit` commands in M3. How to use
those commands is in [workspace.md](workspace.md).

## Building

A solution is built together with a **harness**: a `main()` that `lc`
generates from the problem's signature. It reads a case's inputs, calls
the solution, and writes the result. The build is one source file made of:

1. the **prelude** (`runtime/c/lc_prelude.h` or
   `runtime/cpp/lc_prelude.hpp`): the headers a LeetCode solution expects
   without `#include`, uthash in C, and `using namespace std;` in C++;
2. in C, **lc's versions of a few C library functions**
   (`runtime/c/lc_rt_libc.h`, [below](#the-c-library));
3. `#line 1 "<your file>"`, then **your source**, so the compiler's
   messages give your file's own line numbers;
4. the **runtime** (`runtime/common/lc_rt_core.h`, for a problem with
   lists or trees `runtime/common/lc_rt_nodes.h`, then `runtime/c/lc_rt.c`
   or `runtime/cpp/lc_rt.cpp`): reading and writing the
   [wire format](wire.md) for every type;
5. the **harness**, reported in messages as the file `lc_harness`. An error
   there means the solution's function doesn't match the problem's
   signature; `lc test` then prints the declaration it expected.

The runtime files are compiled into `lc` itself, so `lc` needs nothing
else on disk to build. Because it is one file, the C++ harness sees
`class Solution`. The runtime adds no POSIX header such as `<unistd.h>`,
and calls only functions the prelude's headers declare, so a solution may
name its own helpers `read`, `write`, `sleep` or `nice`, as on LeetCode.

One more file is compiled and linked with it, as a unit of its own:
`runtime/common/lc_rt_stack.c` (`lc_rt_stack.cpp` in a C++ build), which
tells a stack overflow from other crashes ([Verdicts](#verdicts)). It
needs `<signal.h>`, whose names (`kill`, `stack_t`, ...) a solution may
use for its own, so it stays out of the solution's file. The generator
(`gen.c`) is built without it.

**Lists and trees.** For a problem with `ListNode`, `TreeNode` or
`NaryNode`, `lc` defines `LC_LISTNODE`, `LC_TREENODE` or `LC_NARYNODE`
first, and the prelude declares those structs, in LeetCode's form, only
then. The harness builds each list and tree a case gives from its
[wire form](wire.md), and writes the one the solution returns, without
recursion either way, so a list of 10^5 nodes or a tree as deep as it is
long costs the harness no stack. Each node is allocated on its own, with
`malloc` in C (an N-ary node's `children` too, `NULL` when it has none)
and `new` in C++, so a solution may free or delete the nodes it is given,
as on LeetCode; the harness never frees them itself. A problem may
link lists in ways a literal can't say (`build:`) and pass a node of
another input (`ref:`); see
[problem-format.md](problem-format.md#build-and-ref).

**The C library.** LeetCode's C library is glibc. Where another one
behaves differently in a way a solution can see, a C build calls a
version of `lc`'s own, the same on every system:

- **`qsort`** is glibc's merge sort. It is stable: elements that compare
  equal keep their order. And it makes the same comparisons in the same
  order as glibc's, so it also gives the same result with a comparator
  that glibc's sort happens to work with, such as one that returns 1 for
  "greater" and 0 otherwise, never less than 0. The `qsort` of macOS and
  musl is neither, so a solution that relied on either got another answer
  there than on LeetCode.
- **`realloc` and `free`** note when the solution reallocates or frees an
  array it was given, which it may, as on LeetCode
  ([Verdicts](#verdicts)).

Each is a macro for `lc`'s function from there on, so the compiler's
messages about a call to one may name it `lc_rt_qsort`, `lc_rt_realloc` or
`lc_rt_free`. C++ has no such thing: `std::sort` isn't stable, and libc++
(Apple's clang) can leave equal elements in a different order than
libstdc++ (g++, and LeetCode) does. A solution for which that order
matters needs `std::stable_sort`, on LeetCode too.

**Compiling.** C is built with `$CC -std=c11 -O2 -I. ... -lm` and C++
with `$CXX -std=c++11 -O2` (`cc` and `c++` when the variables aren't set,
and the config file's `cc` and `cxx` before either), each through
`/bin/sh`, so a compiler command may have arguments. For C, `lc` puts
`uthash.h` (`runtime/c/uthash.h`) in the build directory, where the
prelude's `#include "uthash.h"` and a solution's own include of it find
it. The standard can be raised (see
[workspace.md](workspace.md#languages-and-standards)); a standard other
than the default is first tried on an empty file, once per compiler, and
the answer is kept in the cache. The config's `cflags` and `cxxflags` are
added at the end of the command for your solutions only, never for the
pack's own sources.

**The build cache.** Builds go in
`$XDG_CACHE_HOME/lcclone/build/<NNNN-slug>/<lang>-<std>/` (by default
`~/.cache/lcclone/...`), as `prog-<key>`; the pack's own sources go in
directories of their own: `ref-<lang>-<std>/`, `generator-c-c11/` for
the full-size cases, and for `lc check` `brute-`, `stub-` and `slow-`
ones ([authoring.md](authoring.md)). The key hashes everything that
goes into the build: the whole source file above, the compiler flags, and
the compiler's path, size and time stamp. Next to each program,
`prog-<key>.log` keeps what the compiler printed. A build whose program
and `.log` are already there is reused, so running the same solution
again costs nothing (a program without its `.log`, from an older `lc`, is
built again). On macOS a build with `-g`, as `--sanitize` builds are, also
has `prog-<key>.dSYM`: Apple's compilers put the debug info there, and the
sanitizers' stack traces need it for their file:line. The last source file
built stays there as `main.c` or `main.cpp`, for looking at. Each build
writes files of its own and renames them into place, so two `lc` processes
never get in each other's way; programs of older builds are removed, with
their `.log` and `.dSYM`, after ten minutes without use.

**Compile Error.** When the compiler fails, nothing runs: the verdict is
Compile Error, with the compiler's output and the messages found in it
(file, line, column, severity, message). Warnings are shown when a build
succeeds too, and again whenever that build is reused: a cached build
carries the output and messages of the build that made its program, so
`lc test` prints the same warnings every time, and an editor reading
`--json` keeps them (only `time_ms` is 0). A warning goes away when the
code that causes it does, not when the build is cached.

## Running a program

Each test case runs in a **new process**, so a crash points at one case
and no global state leaks from one case to the next.

- **Input.** The case's inputs, in the [wire format](wire.md), are written
  to the program's stdin, which is then closed.
- **Output.** stdout and stderr are captured separately. The harness
  writes the function's result on **fd 3**, so whatever the solution
  prints doesn't get in the way.
- **Only fds 0 to 3 are open** in the program, whatever the front-end has
  open.
- **Process group.** The program runs in a process group of its own.
  Killing the group also kills anything the program started, and a ^C at
  the terminal reaches only `lc`, which cancels the run.
- **Signals** that the front-end ignores or blocks are reset to their
  defaults, so a solution that writes to a closed pipe dies of SIGPIPE as
  it would anywhere else.
- **Environment.** The program inherits `lc`'s environment, except that
  `TZ` is `UTC0` (UTC, in POSIX's form, which needs no time zone files):
  whatever the machine's time zone, a program runs in UTC, as on
  LeetCode. A solution that counts days with `mktime` would otherwise be
  a day short across the start of daylight saving time. Every program `lc`
  runs gets this, compilers too. It runs in the directory the caller
  chooses.

The parent writes the input and reads the three outputs in a single
`poll()` loop, so a large input can't deadlock against a large output. It
uses no signal handlers and no `alarm()`. It waits only for its own child,
by PID, and it looks at the cancel flag and the clock at least every
50 ms.

A run is over when the program has exited and its pipes have closed. If
the program exits while something it started still holds a pipe, `lc`
stops waiting within about 50 ms and kills the rest of the group. A
program that closes its outputs and keeps running still runs until it
exits or reaches the deadline.

## Limits

A problem sets `time_limit_ms` (default 1000) and `memory_limit_mb`
(default 256) in its `problem.md`. From them, each case gets:

| What | Limit | Why |
|------|-------|-----|
| CPU time | **`time_limit_ms`**, scaled to this machine ([below](#slower-machines)), user plus system | the time limit itself; checked when the program ends |
| `RLIMIT_CPU` | the time limit rounded up to whole seconds, plus 1 s | stops a program that spins (SIGXCPU, then SIGKILL a second later) |
| Wall clock | 2 × the time limit + 1 s | stops a program that sleeps or blocks; room for a busy machine |
| Peak resident memory | **`memory_limit_mb`** | the memory limit itself; checked when the program ends |
| `RLIMIT_AS` | 2 × the memory limit + 64 MB | stops a runaway program soon after it passes the memory limit |
| `RLIMIT_STACK` | 256 MB, or the system's hard limit if lower | deep recursion, common in list and tree problems |
| `RLIMIT_FSIZE` | 16 MB | solutions have no reason to write files |
| `RLIMIT_CORE` | 0 | no core dumps |
| stdout, stderr | 1 MiB each | more is Output Limit Exceeded |
| Result on fd 3 | 16 MiB | more is Output Limit Exceeded |

**Time** is judged on CPU time, which is steadier than elapsed time on a
loaded machine. Elapsed time is reported too.

**Memory** is judged on the program's peak resident memory, as `wait4()`
reports it. The address-space limit is only a guard: it is set well above
the memory limit, so that a program that goes over the limit is caught by
its peak and not by a failed allocation. For example, a C++ vector that
doubles from 128 MB to 256 MB under a 256 MB limit needs 384 MB while it
copies. A failed allocation would show up as a crash, which says much less
about what went wrong.

Only memory the program has touched counts, as on LeetCode: memory it
asks for and never uses costs nothing. In C, `calloc` of a huge block is
cheap, because the C library gets a large block from the system already
zeroed and the system hands out its pages only when they are first used;
the same allocation as a C++ `vector`, which writes every element, counts
in full. `lc` doesn't count memory that is only reserved: that would turn
away solutions LeetCode accepts, and macOS has no limit that counts it.

The same limits apply on every platform, with these differences:

- **macOS** doesn't enforce `RLIMIT_AS`, so `lc` watches the program's
  memory itself, every 5 ms. It kills the program once the program passes
  the guard.
- **Linux** enforces `RLIMIT_AS`, which counts memory the program has
  reserved, used or not. So there an allocation larger than the guard
  (576 MB under the default limit) fails even when the program would
  barely touch it, and `malloc` returns NULL. On macOS the same allocation
  works, and only the memory touched counts.
- **macOS** caps `RLIMIT_STACK` at 64 MB, so a case gets 64 MB of stack
  there. That holds a recursion 10^5 calls deep with frames of up to
  about 600 bytes, so a recursive solution gets through a list of 10^5
  nodes, or a tree as deep as it is long, on every system, also when
  `lc submit` checks it with the sanitizers.
- **Linux** counts, in a program's peak memory, what it inherited from
  the process that started it (the pages `fork()` copied): `lc`'s,
  megabytes when it holds a full-size case, or a GUI's that links
  `liblc`. So there the harness measures the solution itself: when `lc`
  asks it to (with `LC_RT_MEASURE=4` in its environment and a pipe on fd
  4), the program forks before any of its own code runs. The child runs
  the solution, having inherited only the few pages of a program that has
  just started; the parent waits for it and reports to `lc` how it ended,
  its CPU time and its peak memory. When the program runs out of time,
  `lc` kills the child first, so that the report still comes. macOS and
  the BSDs don't count inherited pages, and need none of this.

### Slower machines

A pack's `time_limit_ms` is written for a **reference machine**, an Apple
M4, and tells methods apart by a factor of about three on either side
([authoring.md](authoring.md#time-limits)). On a machine four times slower,
a method LeetCode accepts would run out of time: 121's `O(n^2)` at 10^4
takes 13 ms on the M4 against a limit of 50 ms. So `lc` multiplies the limit
by a **scale** for the machine it runs on. The CPU limit, the wall-clock
deadline and `RLIMIT_CPU` all follow from the scaled limit.

**Measuring.** `lc` times a fixed loop, built into it
(`src/core/speed_loop.c`): loads that depend on the last one over a
shuffled 128 KiB table, integer mixing, and a branch on a random bit. The
Makefile builds that one file with its own flags, so `CFLAGS` such as `-O0`
or the sanitizers don't change its time, and the loop is written so that
compilers can't vectorize it or remove the branch: Apple clang and gcc 16
are within 12% of each other. It runs 15 rounds of about 6 ms and keeps
the **best** round, in CPU time, because noise only ever makes a round
slower. On the M4, with every core busy, the best round is at most 15%
slower than on a quiet machine.

**When, and where it is kept.** Once per CPU, the first time something is
judged (`lc: note: measuring this machine's speed for the time limits
(once)`, on stderr, about 0.1 s). The time goes in
`$XDG_CACHE_HOME/lcclone/machine-speed`, one line per kind of CPU, so a home
directory shared by several machines works. Judging never measures again
while that line is there, so **the scale doesn't change from one run to the
next**. `lc calibrate` measures again, for example after a first
measurement on battery power; a lock keeps two `lc` processes from
measuring at once.

**From a time to a scale.** The ratio to the reference machine's time
picks one of a few coarse steps:

| The loop takes, against the reference | Scale |
|---|---|
| under 1.35 times (or faster) | 1 |
| 1.35 to 1.75 | 1.5 |
| 1.75 to 2.5 | 2 |
| 2.5 to 3.5 | 3 |
| 3.5 to 5 | 4 |
| 5 to 7 | 6 |
| 7 to 10 | 8 |
| 10 to 14 | 12 |
| 14 or more | 16 |

- **Never below 1.** A faster machine keeps the limits as written: a
  program's start-up doesn't get faster with it, and tighter limits could
  fail right solutions.
- **Coarse on purpose.** A measurement that is off by 10%, or lands on the
  far side of a step, changes the limit by at most about 1.5 times. Limits
  leave three times the room on each side, so that doesn't flip the
  verdict of a method LeetCode clearly accepts or rejects.
- **Capped at 16,** so that a wild measurement (an emulator, say) can't
  make limits meaningless.

**Setting it yourself.** The most specific wins: `--time-scale X` on
`lc test`, `lc submit` and `lc check`, then `LC_TIME_SCALE=X`, then
`time_scale = X` in the [config](workspace.md#configuration). X is a number
from 0.1 to 100, or `auto` for the measured scale. `--time-scale 1` judges
by the limits as written.

**Seeing it.** At scale 1 the output is as it always was. At any other,
`lc test` and `lc submit` end with a line such as `Time limit: 200 ms
(100 ms x 2 for this machine; see 'lc help calibrate')`, a message about
time says `the limit is 200 ms (100 ms x 2 for this machine)`, and `lc
check` starts with `Time limits x 2 on this machine (measured)`. `--json`
gives `time_limit_ms`, `time_scale` and `time_scale_source`
([json.md](json.md#done)). `lc calibrate --show` says what is in use and
why.

**What it can't do.** One loop can't predict every program: a vectorized
loop, or one limited by memory, may slow down by more or less than the
loop does. Neither does the scale follow a machine's speed from moment to
moment, as on a laptop that slows down on battery: it is measured once, so
that verdicts stay put. For both, the limits' threefold room is what's
left, and `--time-scale` is there when it isn't enough.

## Verdicts

A case that ran gets the first verdict below whose rule applies.

| Verdict | When |
|---------|------|
| Output Limit Exceeded | stdout or stderr passed 1 MiB, or the result passed 16 MiB; the program was killed on the spot |
| Memory Limit Exceeded | the peak resident memory passed the memory limit, or `lc` killed the program at the address-space guard |
| Time Limit Exceeded | the CPU time passed the time limit, or `RLIMIT_CPU` stopped the program, or it was still running at the wall-clock deadline |
| Runtime Error | the program ran out of stack (checked before memory: the stack it filled counts in its peak) |
| Runtime Error | the program was killed by a signal, or exited with a status other than 0 |
| Runtime Error | it exited 0 without writing a result, or with a result that isn't valid wire format for the result's type, or with extra text after the result |
| Wrong Answer | the result doesn't match the expected one under the problem's `compare:` mode |
| Accepted | it matches |

Compile Error comes from building (see above): no case runs.

Memory is checked before time because a program that runs out of memory
is often slow too (it may even be swapping), and the memory is the better
explanation. A right answer doesn't excuse a crash: a program that writes
the correct result and then exits with status 1 gets Runtime Error.

Every verdict other than Accepted and Wrong Answer comes with a message
that says why, for example:

- `killed by SIGSEGV`
- `killed by SIGSEGV: stack overflow, past the 64 MB of stack: a recursion
  too deep, or a local array too large`
- `exited with status 1`
- `used 1630 ms of CPU time; the limit is 1000 ms`
- `used 250 ms of CPU time; the limit is 200 ms (100 ms x 2 for this machine)`
- `was still running after 3004 ms (it used 3 ms of CPU time; the limit is 1000 ms)`
- `used 300 MB of memory; the limit is 256 MB`
- `printed more than 1 MiB on stdout`
- `exited without returning a result`
- `its result could not be read: byte 5: expected an int, got 'x'`

`lc submit` then checks an accepted run with the sanitizers, and a case
that fails there gets that verdict instead ([below](#the-sanitizer-check)).

**Stack overflows.** A recursion too deep is a crash like any other, and
LeetCode reports it as a Runtime Error too. `lc` says so where it can
tell: before `main()`, the program gets a handler for SIGSEGV and SIGBUS
(`runtime/common/lc_rt_stack.c`), on a stack of its own. When the address
that faulted lies at the end of the main stack, the handler writes
`lc-rt stack-overflow SIZE` on fd 3, where the harness writes the result
only once the solution has returned, and the program then dies of the
signal as it would have anyway. `lc` turns that into the message above.
Built with AddressSanitizer (`--sanitize`, and `lc submit`'s sanitizer
check), the program has no such handler: ASan reports the overflow itself
on stderr and exits with status 1, and `lc` says `exited with status 1:
AddressSanitizer found a stack overflow: ...`. A crash elsewhere, or a
fault far from the end of the stack, stays `killed by SIGSEGV`.

When the harness can't write the result, because the solution handed back
something it can't use, it stops with status 70 and says why on stderr:
`lc: twoSum did not set *returnSize`, `lc: reverse returned NULL, but
*returnSize is 3`, or for a problem solved in place
([`check: prefix`](problem-format.md#check)) `lc: removeElement returned
6, but nums has 5 elements`. It does the same, in C, when the solution
wrote past the end of an array it was given: `lc: duplicateZeros wrote
past the end of arr (arr[8])`. That is a Runtime Error too, shown with the
message (and `lc check` adds the message to its finding). Built with
AddressSanitizer, the harness leaves out the spare element it catches such
writes with, and ASan reports them itself ([Sanitizers](#sanitizers)).

The harness checks the lists and trees a solution returns as it writes
them, and stops the same way when it can't: a list with a cycle (`lc:
reverseList returned a list with a cycle: its node at index 4 links back
to the one at index 1`), a tree that reaches a node twice, through a
cycle or a node with two parents (`lc: invertTree returned a tree that
reaches a node twice (a cycle, or a node with two parents): the node
whose value is 7`), an N-ary node with a NULL child, or, for a function
that must return a node of one of its inputs
([`ref: return in NAME`](problem-format.md#build-and-ref)), a node that
isn't one of NAME's (`lc: getTargetCopy returned a node that is not one
of cloned's`). LeetCode's harness would run on such a list until it ran
out of time or memory, or, for a node of the wrong input, give Wrong
Answer.

A C solution may `realloc` or `free` an array it was given, as on
LeetCode; say, `realloc(nums, 2 * numsSize * sizeof *nums)` to fill twice
as many elements and return them. The harness then leaves that array
alone, past its end too, since that memory is no longer the array's. The
exception is the array that holds the result of a problem solved in
place: the harness reads it after the call, which on LeetCode reads freed
memory, a `heap-use-after-free`. `lc` stops instead: `lc: merge freed or
reallocated nums1, which holds the result`.

## A run's report

A run goes through its cases in order. Optionally it stops at the first
case that isn't accepted, which is what submitting does. For each case
that finished, the report has:

- the verdict and its message;
- the case's inputs as written, its line in `tests.txt`, and whether it is
  an example;
- the expected result and the program's result, both as canonical
  literals (the program's result is missing when it couldn't be read);
- the first 64 KiB of stdout and the last 64 KiB of stderr, where a crash
  report usually is, each with a flag saying whether it was cut;
- CPU time, elapsed time, peak memory, and the exit status or signal;
- whether it comes from the sanitizer check.

The run's own verdict is that of the first case that failed. It is
Accepted when every case ran and passed.

## Events and cancelling

While a run is under way, the core calls the front-end's callback for
three events:

- `case_start` before each case;
- `case_done` after each case, with its result;
- `done` at the end, with the whole report.

When the core builds the solution itself (`lc_judge`), two more come
first: `build_start` and `build_done`, for the solution and, when your
cases need it, for the reference solution. `lc test --json` and
`lc submit --json` stream all of them as JSON Lines ([json.md](json.md)).
`lc_check` sends build events and one more kind, `finding`, for each
thing it finds wrong with a pack ([authoring.md](authoring.md)).

The front-end can also pass a cancel flag. Setting it, from a signal
handler or another thread, kills the program that is running within about
50 ms. The run then ends. The case that was running is left out of the
report, and the report says it was cancelled.

## What front-ends must do

A process that judges (the `lc` CLI, or a GUI that links `liblc`) must:

- **ignore SIGPIPE** (`signal(SIGPIPE, SIG_IGN)`). When a program stops
  reading its input, the core's writes then fail with EPIPE, which it
  handles, instead of killing the front-end;
- **not ignore SIGCHLD.** If SIGCHLD is set to `SIG_IGN`, the system reaps
  programs before the core can collect their exit status and resource
  usage, and the run fails with an error saying so;
- **cancel on ^C.** The program isn't in the terminal's foreground
  process group, so it doesn't get the terminal's SIGINT. A CLI should
  catch SIGINT and set the cancel flag.

These limits are there to catch accidents such as infinite loops and
runaway memory. They are not a sandbox: you are running your own code.

## Full-size cases

A case at the limits of a problem's constraints can be megabytes, so a
pack's `tests.txt` keeps only a line for it, `@full SEED HASH`
([problem-format.md](problem-format.md#teststxt)), and the case is made
when it is judged:

- **The generator.** `lc` builds the pack's `gen.c` with the C compiler
  (`cc`, even for a C++ solution) and `-ffp-contract=off`, and runs it for
  the seed right before the case runs. The case's input is gone again
  once the case has run.
- **The hash.** The input must have the hash the line gives, else the
  generator made another case here than where the line was written (a C
  library or a compiler that rounds differently, say), and `lc` stops
  with an error that names the seed. That is no verdict on the solution,
  and nothing is recorded. A case of your own may leave the hash out.
- **The expected result** is the line's `=> ...` when it has one. When it
  is too long to keep there, the reference solution works it out on the
  input, as for your own cases (below), and `lc` keeps it in
  `$XDG_CACHE_HOME/lcclone/full/<NNNN-slug>/`, named after the build keys
  of `gen` and `ref` and the seed; the input itself isn't kept, since
  making it again takes milliseconds. So a second run costs nothing more.
- **Showing it.** A full-size case is shown by its seed and the size of
  its long inputs (`nums = 100000 items`, or for a list or tree `root =
  100000 nodes`), and `--json` leaves those inputs out ([json.md](json.md#case_start-case_done)).

## Your cases and the reference solution

A case of your own (`cases.txt`, or `--case FILE`) may leave out its
expected result. `lc` then builds the problem's **reference solution**
(`ref.c` or `ref.cpp` in the pack) with the default standard, runs it on
the case under twice the time limit, and uses its result. When the
reference fails on a case, its answer can't be trusted, so `lc` stops with
an error that names the case: the input most likely breaks the problem's
constraints. A pack without a reference solution can only run your cases
that give their expected result.

## Sanitizers

`--sanitize` builds with `-g -fsanitize=address,undefined
-fno-sanitize-recover=all -fno-omit-frame-pointer`, and for C++ also turns
on the standard library's bounds checks (`-D_GLIBCXX_ASSERTIONS` for
libstdc++, `_LIBCPP_HARDENING_MODE_EXTENSIVE` for libc++). An
out-of-bounds access, a use after free or undefined behaviour then stops
the program, and the case is a Runtime Error with the sanitizer's report in
its stderr.

In C, the harness gives each array it hands the solution exactly its
elements when it is built with AddressSanitizer, not the spare zeroed one
that otherwise catches a write past the end. So ASan sees a read one past
the end, `nums[numsSize]`, and reports it as a `heap-buffer-overflow`, as
LeetCode's does; without ASan that read gets 0. A `char[]` keeps its NUL
either way, so that it is also a string. C++ vectors hold exactly their
elements anyway, and the library's bounds checks catch `nums[n]`.

AddressSanitizer reserves a huge address space and uses far more memory
than the program itself, so a sanitized run has no address-space limit and
no memory verdict, and three times the time limit. Leak detection is off:
a solution written for LeetCode doesn't free what it allocates, and
neither does the harness. musl's gcc has no sanitizers; there the build
fails with the compiler's message.

### The sanitizer check

LeetCode builds C and C++ solutions with AddressSanitizer and UBSan. A
signed overflow that `-O2` folds away, a division by zero (which arm64
doesn't trap), a buffer one byte short or a read one past an array's end
can give right answers in a plain build, and fail there. So when every
case of `lc submit` passes, it checks the solution: it builds it again
with the sanitizers, as `--sanitize` does, and runs the same cases again.

```
Building solution.c (C, c11)
Checking solution.c with the sanitizers

solution.c:1: Runtime Error on case 3 of 22 with the sanitizers (killed by SIGABRT)
  Input:    nums = [2,0,5,3,1,4]
  Expected: 6
  Stderr:
    ==17202==ERROR: AddressSanitizer: heap-buffer-overflow on address ...
    READ of size 4 at 0x603000001c78 thread T0
        #0 0x000104eb4990 in missingNumber solution.c:5
    ...
```

- A case that fails in the check takes the place of its result, and the
  submission gets its verdict. That is mostly a Runtime Error with the
  sanitizer's report, but can be a Wrong Answer: ASan fills what `malloc`
  returns with `0xbe` bytes (its first 4 KiB), so a solution that reads
  memory it never wrote can answer differently.
- **Running out of time doesn't count** in the check: programs built with
  the sanitizers are slower (1.5 to 4 times, measured on full-size cases),
  and the first run judged the time. The check has the limits of
  `--sanitize`: three times the time limit and no memory limit.
- The time and memory reported, and recorded as your best time, are the
  first run's.
- **Where the check can't run,** it is skipped and `lc` says why:
  `lc: note: not checked with the sanitizers: cc has none` (musl's gcc), or
  `programs built with them don't run here (killed by SIGSEGV)` (ASan
  can't start under some kernels' settings). `lc` finds this out once per
  compiler, by building and running an empty program with the sanitizers,
  and keeps the answer in `$XDG_CACHE_HOME/lcclone/sanitizers`; delete the
  file to ask again. A solution that compiles, but not with the
  sanitizers, is skipped the same way: `it doesn't build with them`.
- ^C during the check cancels the submission, which isn't recorded.

The check costs a second build, cached like the first, and a second run
of the cases. Measured on an M4 Mac (Apple clang 21) and in a Linux VM on
it (gcc 16): the build adds 20 to 50 ms in C and 80 to 120 ms in C++ on
the Mac, 50 to 120 ms and 100 to 270 ms on Linux; rerunning 25 small cases
adds about 0.2 s on macOS, where a program built with ASan takes about
8 ms to start, and 30 ms on Linux.

`lc test` has no check, to stay quick; `lc test --sanitize` runs with the
sanitizers instead. `lc submit --sanitize` has none either: its only run
is the one built with the sanitizers, without the memory verdict and with
three times the time limit.
