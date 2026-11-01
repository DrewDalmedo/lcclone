id: 3774
slug: absolute-difference-between-maximum-and-minimum-k-elements
title: Absolute Difference Between Maximum and Minimum K Elements
difficulty: easy
tags: array, sorting
signature: int absDifference(int[] nums, int k)
---
Line the elements of `nums` up from smallest to largest. Equal values stand
side by side, and each copy takes a place of its own. The **front group** is
made of the first `k` places of the line (the `k` smallest elements), and
the **back group** of the last `k` places (the `k` largest).

Return the absolute difference between the sum of the back group and the
sum of the front group. The back group's sum is never the smaller of the
two, so this is simply that sum minus the front group's.

When `2k` is more than the length of `nums`, the two groups overlap in the
middle of the line: an element in a shared place counts in both sums.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
- `1 <= k <= nums.length`
