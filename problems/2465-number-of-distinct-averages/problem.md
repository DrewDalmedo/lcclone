id: 2465
slug: number-of-distinct-averages
title: Number of Distinct Averages
difficulty: easy
tags: array, hash-table, two-pointers, sorting
signature: int distinctAverages(int[] nums)
---
The length `n` of `nums` is even. Line its elements up from smallest to
largest and split them into `n / 2` pairs from the outside in: the first
element of the line goes with the last, the second with the second to
last, and so on, until every element is in exactly one pair. How equal
values are ordered in the line doesn't matter: the pairs hold the same
values either way.

The **midpoint** of a pair is the average of its two values, `(a + b) / 2`,
taken exactly, without rounding. So the pair `2`, `7` has midpoint `4.5`,
which is different from the midpoint `4` of `1`, `7` and from the midpoint
`5` of `3`, `7`.

Return how many different midpoints the pairs have. Pairs with the same
midpoint count once.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `nums.length` is even.
- `0 <= nums[i] <= 100`
