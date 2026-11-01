id: 2574
slug: left-and-right-sum-differences
title: Left and Right Sum Differences
difficulty: easy
tags: array, prefix-sum
signature: int[] leftRightDifference(int[] nums)
---
There are `n` piles of coins in a row, numbered `0` to `n - 1` from left to
right, and pile `i` has `nums[i]` coins. Stand by one pile, say pile `i`,
and count the coins on either side of it: those in piles `0` to `i - 1` to
its left, and those in piles `i + 1` to `n - 1` to its right. Pile `i`
itself is not counted on either side, and a side with no piles on it has
`0` coins.

The **gap** at pile `i` is how many more coins one side has than the
other, that is, the absolute difference of the two counts. It is never
negative.

Return an array of length `n` whose element `i` is the gap at pile `i`.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `1 <= nums[i] <= 10^5`
