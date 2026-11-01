id: 1863
slug: sum-of-all-subset-xor-totals
title: Sum of All Subset XOR Totals
difficulty: easy
tags: bit-manipulation, array, math, backtracking, combinatorics, enumeration
signature: int subsetXORSum(int[] nums)
---
Pick some of the positions of `nums`: any number of them, from none at all
to every one. Combining the numbers at the picked positions with bitwise
XOR (`^`) gives that pick's **XOR total**; picking nothing gives `0`.

An array of length `n` allows `2^n` different picks. Return the sum of the
XOR totals of all of them.

Picks are told apart by position, not by value: if a value occurs twice in
`nums`, picking one copy and picking the other are two different picks, and
both are added to the sum.

@examples

## Constraints

- `1 <= nums.length <= 12`
- `1 <= nums[i] <= 20`
