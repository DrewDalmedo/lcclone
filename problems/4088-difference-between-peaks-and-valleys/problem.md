id: 4088
slug: difference-between-peaks-and-valleys
title: Difference Between Peaks and Valleys
difficulty: easy
tags:
signature: int peakValleyDifference(int[] nums)
---
Let `n` be the length of `nums`. Only the inner indices `i`, with
`1 <= i <= n - 2`, are judged; index `0` and index `n - 1` are never peaks
or valleys, even when they hold the largest or the smallest value.

- An inner index `i` is a **peak** if `nums[i] > nums[i - 1]` and
  `nums[i] > nums[i + 1]`.
- An inner index `i` is a **valley** if `nums[i] < nums[i - 1]` and
  `nums[i] < nums[i + 1]`.

Both comparisons are strict, so an element equal to either of its
neighbours is neither a peak nor a valley.

Add up the values found at the peaks, add up the values found at the
valleys, and return the first total minus the second. A total over no
indices is `0`. It is the values that are added, not the number of
indices, and they may be negative or zero: taking away a negative valley
makes the result larger, and the result itself can be negative.

@examples

## Constraints

- `3 <= nums.length <= 100`
- `-100 <= nums[i] <= 100`
