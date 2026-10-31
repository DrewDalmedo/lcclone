id: 350
slug: intersection-of-two-arrays-ii
title: Intersection of Two Arrays II
difficulty: easy
tags: array, hash-table, two-pointers, binary-search, sorting
signature: int[] intersect(int[] nums1, int[] nums2)
compare: unordered 0
---
Return the values that `nums1` and `nums2` have in common, **counting
repeats**: when a value occurs `a` times in `nums1` and `b` times in `nums2`,
the answer holds it exactly `min(a, b)` times. A value that is missing from
either array is not in the answer.

The values may be returned *in any order*. If the two arrays share no value,
return an empty array.

@examples

## Constraints

- `1 <= nums1.length, nums2.length <= 1000`
- `0 <= nums1[i], nums2[i] <= 1000`

## Follow-up

Can you do it in `O(nums1.length + nums2.length)` time, without sorting
either array?
