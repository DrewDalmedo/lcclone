id: 2956
slug: find-common-elements-between-two-arrays
title: Find Common Elements Between Two Arrays
difficulty: easy
tags: array, hash-table
signature: int[] findIntersectionValues(int[] nums1, int[] nums2)
---
Two players each hold a hand of numbered cards: `nums1` lists the numbers
on the first player's cards and `nums2` those on the second player's. A
hand may hold several cards with the same number. A card is **matched**
when the *other* hand holds at least one card with the same number.

Return an array of two integers: how many cards of `nums1` are matched,
then how many cards of `nums2` are matched.

Every card counts on its own. If `nums1` has three cards numbered `5` and
`nums2` holds a `5`, those three cards add `3` to the first count, and
they add `3` whether `nums2` holds one `5` or several.

@examples

## Constraints

- `1 <= nums1.length, nums2.length <= 100`
- `1 <= nums1[i], nums2[i] <= 100`
