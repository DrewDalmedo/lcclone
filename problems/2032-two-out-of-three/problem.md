id: 2032
slug: two-out-of-three
title: Two Out of Three
difficulty: easy
tags: bit-manipulation, array, hash-table
signature: int[] twoOutOfThree(int[] nums1, int[] nums2, int[] nums3)
compare: unordered 0
---
You are given three lists of integers, `nums1`, `nums2` and `nums3`. Return
every value that is found in **at least two** of the three lists, *in any
order*.

A list counts once for a value however many times it holds it, so a value
repeated within one list but missing from the other two doesn't qualify.
Return each qualifying value only once, even when it is in all three lists.
If no value qualifies, return an empty array.

@examples

## Constraints

- `1 <= nums1.length, nums2.length, nums3.length <= 100`
- `1 <= nums1[i], nums2[j], nums3[k] <= 100`
