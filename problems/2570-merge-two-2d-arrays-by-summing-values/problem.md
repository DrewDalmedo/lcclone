id: 2570
slug: merge-two-2d-arrays-by-summing-values
title: Merge Two 2D Arrays by Summing Values
difficulty: easy
tags: array, hash-table, two-pointers
signature: int[][] mergeArrays(int[][] nums1, int[][] nums2)
---
A shop has two checkouts, and at closing time each one prints a tally of
the day's sales: `nums1` from the first checkout, `nums2` from the second.
Each row of a tally is a pair `[id, val]`, meaning that the checkout sold
`val` units of product number `id`. A tally lists each product at most
once, in increasing order of product number. A product missing from a
tally sold nothing at that checkout.

Produce the shop's tally for the whole day, in the same form. Every
product sold at either checkout, or at both, gets a single row
`[id, total]`: its number first, then the units the two checkouts sold of
it added together. The rows go by increasing product number. Products
that neither checkout sold are not listed.

@examples

## Constraints

- `1 <= nums1.length, nums2.length <= 200`
- `nums1[i].length == nums2[j].length == 2`
- `1 <= nums1[i][0], nums1[i][1], nums2[j][0], nums2[j][1] <= 1000`
- The product numbers `nums1[i][0]` are distinct and strictly increasing,
  and so are the product numbers `nums2[j][0]`.
