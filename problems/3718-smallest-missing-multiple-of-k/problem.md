id: 3718
slug: smallest-missing-multiple-of-k
title: Smallest Missing Multiple of K
difficulty: easy
tags: array, hash-table
signature: int missingMultiple(int[] nums, int k)
---
Write out the positive multiples of `k` from the smallest up: `k`,
`2 * k`, `3 * k`, and so on without end. (`0` is not on this list.) Some of
them may be elements of `nums`. Return the first number on the list that is
not an element of `nums`.

The array `nums` may be unsorted and may hold the same value more than once.
Values that `k` does not divide are not on the list and have no effect on
the answer. No element exceeds `100`, so the answer lies beyond `100`
exactly when all the multiples of `k` from `k` to `100` occur in `nums`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
- `1 <= k <= 100`
