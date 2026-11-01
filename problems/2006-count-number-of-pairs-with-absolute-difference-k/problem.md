id: 2006
slug: count-number-of-pairs-with-absolute-difference-k
title: Count Number of Pairs With Absolute Difference K
difficulty: easy
tags: array, hash-table, counting
signature: int countKDifference(int[] nums, int k)
---
Take two indices `i < j` of `nums`. They make a **k-apart pair** when one of
`nums[i]` and `nums[j]` is exactly `k` more than the other. Either of the
two may be the larger one: the gap between them is what counts, not its
direction.

Return how many k-apart pairs there are.

Each pair of indices is counted once. Different indices are different even
when they hold the same value, so copies multiply: with `k = 3`, two `4`s
and three `7`s give `2 * 3 = 6` pairs. Since `k` is at least 1, two equal
values are never a pair.

@examples

## Constraints

- `1 <= nums.length <= 200`
- `1 <= nums[i] <= 100`
- `1 <= k <= 99`
