id: 2016
slug: maximum-difference-between-increasing-elements
title: Maximum Difference Between Increasing Elements
difficulty: easy
tags: array
signature: int maximumDifference(int[] nums)
---
Choose two positions of `nums`, an earlier one `i` and a later one `j`
(`i < j`), such that the later number is **strictly larger** than the
earlier one: `nums[i] < nums[j]`. Such a choice gains `nums[j] - nums[i]`.

Return the largest gain that any such choice makes. If there is no such
choice at all, because no number of `nums` has a strictly larger number
somewhere after it, return `-1`. Two equal numbers never count, so the
answer is either `-1` or a positive number, never `0`.

The order matters: a large number that comes *before* a small one can't
be paired with it.

@examples

## Constraints

- `2 <= nums.length <= 1000`
- `1 <= nums[i] <= 10^9`
