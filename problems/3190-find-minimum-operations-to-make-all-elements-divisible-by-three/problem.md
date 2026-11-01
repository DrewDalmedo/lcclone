id: 3190
slug: find-minimum-operations-to-make-all-elements-divisible-by-three
title: Find Minimum Operations to Make All Elements Divisible by Three
difficulty: easy
tags: array, math
signature: int minimumOperations(int[] nums)
---
`nums` is a list of positive integers. You may **nudge** its elements: one
nudge picks a single element and either raises it by `1` or lowers it by
`1`. The same element can be nudged as many times as you like.

Return the smallest number of nudges after which every element of `nums` is
a multiple of `3`. Each element only has to become a multiple of `3` on its
own; the elements don't need to end up equal. `0` is a multiple of `3`, so
an element may be lowered all the way to `0`.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 50`
