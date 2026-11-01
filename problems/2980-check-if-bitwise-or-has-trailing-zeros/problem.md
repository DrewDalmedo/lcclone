id: 2980
slug: check-if-bitwise-or-has-trailing-zeros
title: Check if Bitwise OR Has Trailing Zeros
difficulty: easy
tags: bit-manipulation, array
signature: bool hasTrailingZeros(int[] nums)
---
The **trailing zeros** of a positive integer are the `0` digits at the
right-hand end of its binary form, after its last `1`. For example, `20` is
`10100` in binary and has two trailing zeros, while `7`, which is `111`, has
none.

Choose **two or more** elements of `nums`, each one at most once, and
combine all of them with bitwise OR (`|`). Return `true` if some choice
gives a result with at least one trailing zero, and `false` if none does.

Each position of `nums` holds its own element: if a value appears twice,
a choice can include both copies.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
