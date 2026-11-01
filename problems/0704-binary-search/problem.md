id: 704
slug: binary-search
title: Binary Search
difficulty: easy
tags: array, binary-search
signature: int search(int[] nums, int target)
---
Look up `target` in `nums`, an array of integers in which every element is
larger than the one before it. If an element equals `target`, return its
index, counting from `0`; no value repeats, so there is at most one such
index. If no element equals `target`, return `-1`, not the place where
`target` would fit.

Your search must take `O(log n)` time on an array of `n` elements, so
reading the elements one by one from either end is out: each comparison
should cut the candidates still left to about half. With at most `10^4`
elements, a front-to-back scan also finishes well within the time limit,
so passing the tests doesn't show that a search is fast enough. The
`O(log n)` bound is part of the task all the same.

@examples

## Constraints

- `1 <= nums.length <= 10^4`
- `-10^4 < nums[i] < 10^4`
- `-10^4 < target < 10^4`
- `nums` is strictly increasing.
