id: 2798
slug: number-of-employees-who-met-the-target
title: Number of Employees Who Met the Target
difficulty: easy
tags: array
signature: int numberOfEmployeesWhoMetTarget(int[] hours, int target)
---
A team's monthly time sheet gives, for each member of the team, the
number of hours they logged: member `i` logged `hours[i]` hours. The
team's rule is that everyone logs at least `target` hours in the month.

Return how many members kept to the rule, that is, how many indices `i`
have `hours[i] >= target`. Landing exactly on `target` is enough, so
when `target` is `0` every member counts, even one who logged `0` hours.

@examples

## Constraints

- `1 <= hours.length <= 50`
- `0 <= hours[i] <= 10^5`
- `0 <= target <= 10^5`
