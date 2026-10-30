id: 4
slug: median-of-two-sorted-arrays
title: Median of Two Sorted Arrays
difficulty: hard
tags: array, binary-search, divide-and-conquer
signature: double findMedianSortedArrays(int[] nums1, int[] nums2)
compare: float 1e-5
---
Two arrays, `nums1` and `nums2`, are each sorted in non-decreasing order.
Picture them merged into a single sorted array, and return the **median** of
that merged array.

The median of a sorted list is:

1. its middle element, when the length is odd;
2. the mean of its two middle elements, when the length is even.

At least one of the two arrays is non-empty.

@examples

## Constraints

- `0 <= nums1.length, nums2.length <= 1000`
- `1 <= nums1.length + nums2.length`
- `-10^6 <= nums1[i], nums2[i] <= 10^6`

> Aim for `O(log(m + n))` time, where `m` and `n` are the two lengths. A
> linear merge passes too, but misses the point of the problem.
