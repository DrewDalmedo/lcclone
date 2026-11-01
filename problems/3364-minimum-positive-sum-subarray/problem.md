id: 3364
slug: minimum-positive-sum-subarray
title: Minimum Positive Sum Subarray
difficulty: easy
tags: array, prefix-sum, sliding-window
signature: int minimumSumSubarray(int[] nums, int l, int r)
---
A **window** of `nums` is a stretch of one or more neighbouring elements,
`nums[i]`, `nums[i + 1]`, ..., `nums[j]`. Its **length** is the number of
elements it covers, `j - i + 1`, and its **total** is their sum.

Look only at the windows whose length is at least `l` and at most `r`. Of
those, keep the ones whose total is `1` or more; a total of exactly `0` is
not kept. Return the smallest total among the windows kept, or `-1` if no
window is kept.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= l <= r <= nums.length`
- `-1000 <= nums[i] <= 1000`
