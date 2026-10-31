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
2. `#line 1 "<your file>"`, then **your source**, so the compiler's
   messages give your file's own line numbers;
3. the **runtime** (`runtime/common/lc_rt_core.h`, then `runtime/c/lc_rt.c`
   or `runtime/cpp/lc_rt.cpp`): reading and writing the
   [wire format](wire.md) for every type;
4. the **harness**, reported in messages as the file `lc_harness`. An error
   there means the solution's function doesn't match the problem's
   signature; `lc test` then prints the declaration it expected.

The runtime files are compiled into `lc` itself, so `lc` needs nothing
else on disk to build. Because it is one file, the C++ harness sees
`class Solution`.

**Compiling.** C is built with `$CC -std=c11 -O2 -I. ... -lm` and C++
with `$CXX -std=c++11 -O2` (`cc` and `c++` when the variables aren't set),
each through `/bin/sh`, so a compiler command may have arguments. For C,
`lc` puts `uthash.h` (`runtime/c/uthash.h`) in the build directory, where
the prelude's `#include "uthash.h"` and a solution's own include of it
find it. The standard
can be raised (see [workspace.md](workspace.md#languages-and-standards));
a standard other than the default is first tried on an empty file, once
per compiler, and the answer is kept in the cache.

**The build cache.** Builds go in
`$XDG_CACHE_HOME/lcclone/build/<NNNN-slug>/<lang>-<std>/` (by default
`~/.cache/lcclone/...`), as `prog-<key>`. The key hashes everything that
goes into the build: the whole source file above, the compiler flags, and
the compiler's path, size and time stamp. A build whose program is
already there is reused, so running the same solution again costs nothing.
The last source file built stays there as `main.c` or `main.cpp`, for
looking at. Each build writes files of its own and renames the program
into place, so two `lc` processes never get in each other's way; programs
of older builds are removed after ten minutes without use.

**Compile Error.** When the compiler fails, nothing runs: the verdict is
Compile Error, with the compiler's output and the messages found in it
(file, line, column, severity, message). Warnings are shown when a build
succeeds too, the first time.

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
- **Environment.** The program inherits `lc`'s environment and runs in the
  directory the caller chooses.

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
| CPU time | **`time_limit_ms`**, user plus system | the time limit itself; checked when the program ends |
| `RLIMIT_CPU` | the time limit rounded up to whole seconds, plus 1 s | stops a program that spins (SIGXCPU, then SIGKILL a second later) |
| Wall clock | 2 × the time limit + 1 s | stops a program that sleeps or blocks; room for a busy machine |
| Peak resident memory | **`memory_limit_mb`** | the memory limit itself; checked when the program ends |
| `RLIMIT_AS` | 2 × the memory limit + 64 MB | stops a runaway program soon after it passes the memory limit |
| `RLIMIT_STACK` | 256 MB, or the system's hard limit if lower | deep recursion, common in tree problems |
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

The same limits apply on every platform, with these differences:

- **macOS** doesn't enforce `RLIMIT_AS`, so `lc` watches the program's
  memory itself, every 5 ms. It kills the program once the program passes
  the guard.
- **macOS** caps `RLIMIT_STACK` at 64 MB, so a case gets 64 MB of stack
  there.
- **Linux** counts, in a program's peak memory, what it inherited from
  `lc` before it started (the pages `fork()` copied). For `lc` itself that
  is a few MB. A front-end that links `liblc` and holds a lot of memory
  should keep this in mind.

## Verdicts

A case that ran gets the first verdict below whose rule applies.

| Verdict | When |
|---------|------|
| Output Limit Exceeded | stdout or stderr passed 1 MiB, or the result passed 16 MiB; the program was killed on the spot |
| Memory Limit Exceeded | the peak resident memory passed the memory limit, or `lc` killed the program at the address-space guard |
| Time Limit Exceeded | the CPU time passed the time limit, or `RLIMIT_CPU` stopped the program, or it was still running at the wall-clock deadline |
| Runtime Error | the program was killed by a signal, or exited with a status other than 0 |
| Runtime Error | it exited 0 without writing a result, or with a result that isn't valid wire format for the return type, or with extra text after the result |
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
- `exited with status 1`
- `used 1630 ms of CPU time; the limit is 1000 ms`
- `was still running after 3004 ms (it used 3 ms of CPU time; the limit is 1000 ms)`
- `used 300 MB of memory; the limit is 256 MB`
- `printed more than 1 MiB on stdout`
- `exited without returning a result`
- `its result could not be read: byte 5: expected an int, got 'x'`

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
- CPU time, elapsed time, peak memory, and the exit status or signal.

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

AddressSanitizer reserves a huge address space and uses far more memory
than the program itself, so a sanitized run has no address-space limit and
no memory verdict, and three times the time limit. Leak detection is off:
a solution written for LeetCode doesn't free what it allocates, and
neither does the harness. musl's gcc has no sanitizers; there the build
fails with the compiler's message.
