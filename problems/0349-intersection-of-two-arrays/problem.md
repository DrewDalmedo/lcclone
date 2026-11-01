id: 349
slug: intersection-of-two-arrays
title: Intersection of Two Arrays
difficulty: easy
tags: array, hash-table, two-pointers, binary-search, sorting
signature: int[] intersection(int[] nums1, int[] nums2)
compare: unordered 0
---
Return the values that `nums1` and `nums2` have in common: a value belongs
to the answer when it occurs at least once in `nums1` and at least once in
`nums2`.

List each such value **exactly once**, however many times either array
repeats it. The values may be returned *in any order*. If the two arrays
share no value, return an empty array.

@examples

## Constraints

- `1 <= nums1.length, nums2.length <= 1000`
- `0 <= nums1[i], nums2[i] <= 1000`

## Follow-up

The values are small. Can you find the answer in one pass over each array,
without sorting?
