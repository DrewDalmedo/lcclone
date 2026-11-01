id: 2206
slug: divide-array-into-equal-pairs
title: Divide Array Into Equal Pairs
difficulty: easy
tags: bit-manipulation, array, hash-table, counting
signature: bool divideArray(int[] nums)
---
A laundry basket holds an even number of socks, and `nums[i]` is the colour
code of sock `i`. You want to fold every sock in the basket into a bundle of
exactly two socks of the same colour, leaving no sock over. Any two socks of
one colour can share a bundle, wherever they lie in `nums`, and a colour may
fill several bundles: four socks of one colour make two bundles.

Return `true` if the whole basket can be bundled this way, and `false` if it
can't.

@examples

## Constraints

- `nums.length == 2 * n`
- `1 <= n <= 500`
- `1 <= nums[i] <= 500`
