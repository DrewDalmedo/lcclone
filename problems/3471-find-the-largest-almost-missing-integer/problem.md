id: 3471
slug: find-the-largest-almost-missing-integer
title: Find the Largest Almost Missing Integer
difficulty: easy
tags: array, hash-table
signature: int largestInteger(int[] nums, int k)
---
Let `n` be the length of `nums`. A window of width `k` can be laid over
`nums` in `n - k + 1` places: starting at index `0`, `1`, ..., `n - k`,
each time covering `k` consecutive elements.

For a value `x`, count the places where the window covers at least one
copy of `x`. Several copies of `x` under the window in one place still
count once, and two different places count as two even when they cover
the same numbers. A value that doesn't occur in `nums` has a count of 0.

Return the largest value whose count is exactly `1`. If no value has a
count of `1`, return `-1`.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `0 <= nums[i] <= 50`
- `1 <= k <= nums.length`
