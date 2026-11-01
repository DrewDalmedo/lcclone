id: 2859
slug: sum-of-values-at-indices-with-k-set-bits
title: Sum of Values at Indices With K Set Bits
difficulty: easy
tags: bit-manipulation, array
signature: int sumIndicesWithKSetBits(int[] nums, int k)
---
`nums` holds `n` values, at places (indices) numbered from `0` up to
`n - 1`. Write each place number in binary and look at how many of its
digits are `1`: place `0` has none, place `12` (`1100`) has two, and place
`14` (`1110`) has three.

Add up the values stored at the places whose number has exactly `k` digits
equal to `1`, and return the total. When no place qualifies, the total is
`0`.

A place qualifies or not because of its own number, whatever value is
stored there: the binary digits of the values play no part.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `1 <= nums[i] <= 10^5`
- `0 <= k <= 10`
