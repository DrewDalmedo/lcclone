id: 3105
slug: longest-strictly-increasing-or-strictly-decreasing-subarray
title: Longest Strictly Increasing or Strictly Decreasing Subarray
difficulty: easy
tags: array
signature: int longestMonotonicSubarray(int[] nums)
---
A **segment** of `nums` is a run of one or more neighbouring elements,
`nums[i], nums[i + 1], ..., nums[j]`. A segment is **rising** when every
element after its first is larger than the element on its left, and
**falling** when every element after its first is smaller than the element
on its left. A segment of one element is both rising and falling.

Return the number of elements in the longest segment that is rising or
falling.

A repeated value, such as the `5, 5` in `[1,5,5,8]`, stops a rise and a
fall alike: no rising or falling segment holds both copies. Where the
values turn around, the element at the turn is shared: in `[2,6,4]`, the
segment `2, 6` is rising and `6, 4` is falling.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 50`
