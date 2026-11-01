id: 1403
slug: minimum-subsequence-in-non-increasing-order
title: Minimum Subsequence in Non-Increasing Order
difficulty: easy
tags: greedy, array, sorting
signature: int[] minSubsequence(int[] nums)
---
`nums` lists the values of some items. You keep some of the items and hand
over the rest, and what you keep must be worth **strictly more** than what
you hand over: keeping exactly half the total value is not enough.

Keep as few items as possible. If several choices of that many items work,
keep the one worth the most. The kept items are the subsequence of the
title; which entries of `nums` they are matters, not their order.

Return the values of the kept items sorted from largest to smallest. Two
kept items with the same value are both listed. These rules always lead to
the same list of values, so the answer is unique.

@examples

## Constraints

- `1 <= nums.length <= 500`
- `1 <= nums[i] <= 100`
