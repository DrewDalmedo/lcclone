id: 3678
slug: smallest-absent-positive-greater-than-average
title: Smallest Absent Positive Greater Than Average
difficulty: easy
tags: array, hash-table
signature: int smallestAbsent(int[] nums)
---
Add up all `n` entries of `nums`, negative ones and repeated ones included,
and divide the total by `n`. The exact quotient, a fraction that is not
rounded in any way, is the **average** of `nums`.

A whole number `x` is a **candidate** when it passes all three of these
tests:

- `x >= 1`;
- `x` is strictly greater than the average;
- `x` is not equal to any entry of `nums`.

Return the smallest candidate. When the average is a whole number, it fails
the second test, even if it is missing from `nums`. Candidates are not
limited to the range of `nums`: for `[3,3]` the average is `3`, and the
answer is `4`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `-100 <= nums[i] <= 100`
