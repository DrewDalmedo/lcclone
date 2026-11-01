id: 2760
slug: longest-even-odd-subarray-with-threshold
title: Longest Even Odd Subarray With Threshold
difficulty: easy
tags: array, sliding-window
signature: int longestAlternatingSubarray(int[] nums, int threshold)
---
Call a block of neighbouring elements of `nums` a **chain** when it passes
all three of these tests:

- its first element is even;
- the parity changes at every step inside it: no two neighbours in the
  block are both even or both odd, so it reads even, odd, even, odd, ...;
- none of its elements is larger than `threshold` (an element equal to
  `threshold` is fine).

One element can already make a chain: any even element no larger than
`threshold` counts, with length 1. Find the longest chain in `nums` and
return how many elements it holds. If `nums` has no chain at all, which
happens exactly when no even element is at or below `threshold`, return
`0`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
- `1 <= threshold <= 100`
