id: 3852
slug: smallest-pair-with-different-frequencies
title: Smallest Pair With Different Frequencies
difficulty: easy
tags: array, hash-table, counting
signature: int[] minDistinctFreqPair(int[] nums)
---
Stack up the entries of `nums` so that equal numbers share a stack: every
number that occurs in `nums` gets one stack, and its **height** is the
number of entries holding that number. A number that does not occur in
`nums` has no stack, so it can never be part of the answer.

Look for two stacks of different heights; which of the two is taller does
not matter. Name their numbers `x` and `y` with `x < y`. Out of all such
pairs, return the one with the smallest `x`, as `[x, y]`, and break a tie
on `x` by the smallest `y`.

If every stack has the same height (in particular, if `nums` holds only
one distinct number), no such pair exists: return `[-1, -1]`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
