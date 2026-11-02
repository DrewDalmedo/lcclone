id: 3875
slug: construct-uniform-parity-array-i
title: Construct Uniform Parity Array I
difficulty: easy
tags: array, math
signature: bool uniformArray(int[] nums1)
---
`nums1` holds `n` different positive integers. You are to fill in a second
list of `n` numbers, one entry for each position `i` of `nums1`, and for
each position you have this choice:

- copy `nums1[i]` as it is, or
- take `nums1[i] - nums1[j]`, where `j` is any position other than `i`.

The choices are independent of each other. Every difference is taken
between values of the original `nums1`, which never changes, and nothing
stops several positions from subtracting the same `nums1[j]`. A difference
may be negative.

Return `true` if you can make your choices so that the second list is
**uniform**: all of its entries are even, or all of them are odd. Return
`false` if no way of choosing gives a uniform list. Here a number is even
when it is a multiple of 2 and odd otherwise, negative numbers included:
`-6` is even and `-5` is odd.

@examples

## Constraints

- `1 <= n == nums1.length <= 100`
- `1 <= nums1[i] <= 100`
- The values in `nums1` are pairwise distinct.
