id: 2432
slug: the-employee-that-worked-on-the-longest-task
title: The Employee That Worked on the Longest Task
difficulty: easy
tags: array
signature: int hardestWorker(int n, int[][] logs)
---
The `n` employees of a workshop, numbered `0` to `n - 1`, share a single
workbench, where one task is done at a time. Its logbook lists the tasks
in the order they were done: row `logs[i] = [id, end]` says that employee
`id` did task `i` and finished it at minute `end`. Some employees may
never appear in the log.

The bench is never left idle. Task `0` starts at minute `0`, and every
later task starts at the very minute the task before it was finished. In
other words, the length of a task is its finishing minute minus the
finishing minute of the task before it, with `0` standing in for the
finish before task `0`.

Return the id of the employee who did the longest task. If several tasks
tie for the longest time, look at the employees who did them and return
the lowest of their ids, even when a task by a higher id comes earlier in
the log. Each task is timed on its own: the minutes of an employee's
different tasks are never added together.

@examples

## Constraints

- `2 <= n <= 500`
- `1 <= logs.length <= 500`
- `logs[i].length == 2`
- `0 <= logs[i][0] <= n - 1`
- `1 <= logs[i][1] <= 500`
- `logs[i][0] != logs[i + 1][0]`: no employee does two tasks in a row.
- `logs[i][1] < logs[i + 1][1]`
