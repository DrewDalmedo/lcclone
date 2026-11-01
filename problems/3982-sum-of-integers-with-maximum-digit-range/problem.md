id: 3982
slug: sum-of-integers-with-maximum-digit-range
title: Sum of Integers with Maximum Digit Range
difficulty: easy
tags: array, math
signature: int maxDigitRange(int[] nums)
---
Subtracting a positive integer's smallest decimal digit from its largest
gives its **digit range**. A `0` is as much a digit as any other, so
`6082` has digit range `8 - 0 = 8`; `3553` has `5 - 3 = 2`, and `777`,
built from a single repeated digit, has digit range `0`.

Find the largest digit range among the entries of `nums`, and return the
sum of every entry whose digit range equals it.

Several entries may share that largest digit range, and all of them go
into the sum. Repeats are not merged: a value written two or more times
in `nums` is added as often as it is written.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `10 <= nums[i] <= 10^5`
