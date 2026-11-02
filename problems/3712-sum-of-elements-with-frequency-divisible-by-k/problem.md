id: 3712
slug: sum-of-elements-with-frequency-divisible-by-k
title: Sum of Elements With Frequency Divisible by K
difficulty: easy
tags: array, hash-table, counting
signature: int sumDivisibleByK(int[] nums, int k)
---
Gather the entries of `nums` into groups of equal values: one group for
each different value that occurs in `nums`, holding every entry with that
value. The size of a group is how many times its value occurs.

A group **survives** when its size is a multiple of `k`, that is `k`,
`2 * k`, `3 * k`, and so on; every other group is thrown away. Values that
don't occur in `nums` have no group at all.

Return the sum of all the entries in the surviving groups. Every entry
counts, so a surviving group of three `5`s adds `15` to the sum, not `5`.
If no group survives, return `0`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
- `1 <= k <= 100`
