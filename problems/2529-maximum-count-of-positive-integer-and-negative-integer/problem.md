id: 2529
slug: maximum-count-of-positive-integer-and-negative-integer
title: Maximum Count of Positive Integer and Negative Integer
difficulty: easy
tags: array, binary-search, counting
signature: int maximumCount(int[] nums)
---
`nums` holds the balances of a bank's accounts in order from lowest to
highest; several accounts may have the same balance.

An account is **overdrawn** when its balance is below `0`, and **in
credit** when its balance is above `0`. An account with a balance of
exactly `0` is neither, and is left out of both counts.

Count the overdrawn accounts and the accounts in credit, and return the
larger of the two counts (their common value when they are equal). If
every balance is `0`, the answer is `0`.

@examples

## Constraints

- `1 <= nums.length <= 2000`
- `-2000 <= nums[i] <= 2000`
- `nums` is sorted in non-decreasing order.

## Follow-up

Because of the order, the overdrawn accounts come first, then those at
`0`, then those in credit. With `n` balances, can you find where each
group starts in `O(log n)` time, instead of looking at every balance?
