id: 3688
slug: bitwise-or-of-even-numbers-in-an-array
title: Bitwise OR of Even Numbers in an Array
difficulty: easy
tags: bit-manipulation, array, simulation
signature: int evenNumberBitwiseORs(int[] nums)
---
Each entry of `nums` is either even or odd. Throw the odd ones away and
merge what is left with the bitwise OR operator (`|`): bit `b` of the
merged value is `1` if some even entry has a `1` in bit `b`, and `0` if
none does. Neither the order of the entries nor repeated values change it.
Return the merged value.

Whether an entry counts is decided by its **value**, not by the index it
sits at. If every entry is odd, nothing is left to merge, and the answer is
`0`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
