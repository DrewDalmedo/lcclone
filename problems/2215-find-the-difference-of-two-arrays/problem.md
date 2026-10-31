id: 2215
slug: find-the-difference-of-two-arrays
title: Find the Difference of Two Arrays
difficulty: easy
tags: array, hash-table
signature: int[][] findDifference(int[] nums1, int[] nums2)
compare: unordered 1
---
Compare the values of two arrays of integers, `nums1` and `nums2`, and
return an array of exactly two arrays:

1. the values that occur in `nums1` but nowhere in `nums2`;
2. the values that occur in `nums2` but nowhere in `nums1`.

Each array lists a value **at most once**, however many times it is repeated
in the input. The two arrays must come in this order, but the values inside
each one may be *in any order*. Either array, or both, may be empty; the
answer still holds two arrays.

@examples

## Constraints

- `1 <= nums1.length, nums2.length <= 1000`
- `-1000 <= nums1[i], nums2[i] <= 1000`
