id: 88
slug: merge-sorted-array
title: Merge Sorted Array
difficulty: easy
tags: array, two-pointers, sorting
signature: void merge(int[] nums1, int m, int[] nums2, int n)
check: param nums1
---
`nums2` holds `n` values, and `nums1` holds `m` values followed by `n`
zeros, so its length is `m + n`. The zeros are not data, just spare slots.
The `m` values of `nums1` are sorted in non-decreasing order, and so are
the values of `nums2`.

Merge `nums2` into `nums1` **in place**: afterwards, all `m + n` elements of
`nums1` must hold the `m` values of `nums1` and the `n` values of `nums2`,
duplicates included, in non-decreasing order.

The function returns nothing: the whole of `nums1` after the call is
checked, and the examples show it as the output. `nums2` is not checked.

@examples

## Constraints

- `nums1.length == m + n`
- `nums2.length == n`
- `0 <= m, n <= 200`
- `1 <= m + n <= 200`
- `-10^9 <= nums1[i], nums2[j] <= 10^9`
- `nums1[0..m-1]` and `nums2` are sorted in non-decreasing order, and the
  last `n` elements of `nums1` are `0`.

## Follow-up

Can you do it in `O(m + n)` time, without a second array to merge into?
