id: 3131
slug: find-the-integer-added-to-array-i
title: Find the Integer Added to Array I
difficulty: easy
tags: array
signature: int addedInteger(int[] nums1, int[] nums2)
---
The array `nums2` was made from `nums1` in two steps. First, one and the
same integer `x` was added to every element of `nums1`. That `x` can be
any integer: below zero it makes every element smaller, and zero leaves
them as they were. Then the resulting values were put in some order,
possibly a different one from before.

So `nums2` holds exactly the values `nums1[i] + x`, each as many times as
it occurs among them, but not necessarily at position `i`. Given both
arrays, return `x`. The input always comes from such a pair, and only one
value of `x` fits it.

@examples

## Constraints

- `1 <= nums1.length == nums2.length <= 100`
- `0 <= nums1[i], nums2[i] <= 1000`
- Some integer `x` turns `nums1` into `nums2` as described.
