id: 1848
slug: minimum-distance-to-the-target-element
title: Minimum Distance to the Target Element
difficulty: easy
tags: array
signature: int getMinDistance(int[] nums, int target, int start)
---
Think of `nums` as a row of tickets, numbered by position from `0` at the
left, with the value `nums[i]` printed on ticket `i`. The same value may
be printed on several tickets. A marker sits on ticket `start`.

Two tickets `i` and `j` are `|i - j|` places apart. Among the tickets
that show the value `target` (there is at least one), take the one
closest to the marker, and return how many places apart it and the
marker are.

Only that count matters: not which ticket it is, nor on which side of
the marker it lies. When the marker's own ticket shows `target`, the
answer is `0`.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `1 <= nums[i] <= 10^4`
- `0 <= start < nums.length`
- `target` occurs in `nums` at least once.
