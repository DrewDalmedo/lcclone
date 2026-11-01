id: 628
slug: maximum-product-of-three-numbers
title: Maximum Product of Three Numbers
difficulty: easy
tags: array, math, sorting
signature: int maximumProduct(int[] nums)
---
Pick three entries of `nums` at three different indices, and multiply the
three values together. Return the largest result that such a pick can
give.

The indices must differ, but the values at them may be equal. `nums` may
hold negative values and zeros, so the answer may be negative or zero.

@examples

## Constraints

- `3 <= nums.length <= 10^4`
- `-1000 <= nums[i] <= 1000`

## Follow-up

Sorting `nums` works, but isn't needed. Can you get the answer by reading
each element once, while keeping only a fixed handful of variables?
