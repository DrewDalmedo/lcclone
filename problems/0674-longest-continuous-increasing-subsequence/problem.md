id: 674
slug: longest-continuous-increasing-subsequence
title: Longest Continuous Increasing Subsequence
difficulty: easy
tags: array
signature: int findLengthOfLCIS(int[] nums)
---
A **block** of `nums` is a single element, or several elements that sit
next to each other in `nums` with none left out between them. A block
**climbs** when each of its elements, after the first, is strictly
greater than the element just before it.

Return how many elements the longest climbing block of `nums` has. A
block of one element climbs, so the answer is always at least `1`.

Two equal neighbours are never in the same climbing block. Skipping
elements isn't allowed either: in `[2,9,3,4]` the values `2`, `3`, `4`
increase, but `9` sits between them, so they don't form a block.

@examples

## Constraints

- `1 <= nums.length <= 10^4`
- `-10^9 <= nums[i] <= 10^9`
