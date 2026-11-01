id: 2441
slug: largest-positive-integer-that-exists-with-its-negative
title: Largest Positive Integer That Exists With Its Negative
difficulty: easy
tags: array, hash-table, two-pointers, sorting
signature: int findMaxK(int[] nums)
---
The array `nums` holds nonzero integers. A positive number `k` is
**mirrored** in `nums` when `k` occurs in `nums` and its opposite `-k` occurs
in `nums` as well. Where the two sit, and which comes first, makes no
difference, and neither does how many copies of each there are: one `k`
and one `-k` are enough.

Return the largest mirrored number. If no positive number is mirrored, for
instance because every value has the same sign, return `-1`.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `-1000 <= nums[i] <= 1000`
- `nums[i] != 0`
