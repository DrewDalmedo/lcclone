id: 3684
slug: maximize-sum-of-at-most-k-distinct-elements
title: Maximize Sum of At Most K Distinct Elements
difficulty: easy
tags: greedy, array, hash-table, sorting
signature: int[] maxKDistinct(int[] nums, int k)
---
You may take up to `k` entries of `nums`, with one restriction: no two of
the entries you take may hold the same value. A value that occurs several
times in `nums` can still be taken only once.

Of all the selections that keep to this rule, find the one whose values
have the largest total, and return its values sorted from largest to
smallest.

Since every entry is positive, exactly one set of values is best. When
`nums` has fewer than `k` different values, that set holds fewer than `k`
values, and the returned array is shorter than `k`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 10^9`
- `1 <= k <= nums.length`
