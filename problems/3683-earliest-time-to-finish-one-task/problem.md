id: 3683
slug: earliest-time-to-finish-one-task
title: Earliest Time to Finish One Task
difficulty: easy
tags:
signature: int earliestTime(int[][] tasks)
---
Row `i` of `tasks` is a pair `[s, t]` for task `i`: work on it begins at
time `s` and lasts `t` time units (its length) without a break, so the task
is done at time `s + t`.

Every task has a worker of its own. The tasks run side by side and
independently: none of them waits for another, and they may overlap in any
way. Return the smallest value of `s + t` over all the rows: before that
time no task is done yet, and at that time one task, or several, are done.

@examples

## Constraints

- `1 <= tasks.length <= 100`
- `tasks[i].length == 2`
- `1 <= tasks[i][0], tasks[i][1] <= 100`
