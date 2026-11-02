id: 3512
slug: minimum-operations-to-make-array-sum-divisible-by-k
title: Minimum Operations to Make Array Sum Divisible by K
difficulty: easy
tags: array, math
signature: int minOperations(int[] nums, int k)
---
Picture `nums` as a row of piles of stones, where pile `i` holds `nums[i]`
stones. A **move** takes a single stone off any pile that still has one.
You may take from one pile over and over, or from several piles, in any
order you like.

You want the number of stones left, counted over all the piles together,
to be a multiple of `k`. Return the fewest moves that get you there.

Clearing the table is allowed: no stones at all, a total of `0`, counts as
a multiple of `k`. When the starting total is already a multiple of `k`,
the answer is `0`.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `1 <= nums[i] <= 1000`
- `1 <= k <= 100`
