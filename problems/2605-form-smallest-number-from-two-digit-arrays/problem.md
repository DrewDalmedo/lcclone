id: 2605
slug: form-smallest-number-from-two-digit-arrays
title: Form Smallest Number From Two Digit Arrays
difficulty: easy
tags: array, hash-table, enumeration
signature: int minNumber(int[] nums1, int[] nums2)
---
`nums1` and `nums2` are two lists of digits from `1` to `9`. No digit
repeats within one list, but the same digit may be in both lists. The
digits of a list come in no particular order.

A positive integer **covers** both lists when its decimal digits include
at least one digit from `nums1` and at least one digit from `nums2`. One
digit may count for both lists at once: if `d` is in both, the number `d`
on its own covers them.

Return the smallest positive integer that covers both lists.

@examples

## Constraints

- `1 <= nums1.length, nums2.length <= 9`
- `1 <= nums1[i], nums2[i] <= 9`
- The digits within `nums1` are distinct, and so are the digits within
  `nums2`.
