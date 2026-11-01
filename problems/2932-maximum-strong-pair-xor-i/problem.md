id: 2932
slug: maximum-strong-pair-xor-i
title: Maximum Strong Pair XOR I
difficulty: easy
tags: bit-manipulation, trie, array, hash-table, sliding-window
signature: int maximumStrongPairXor(int[] nums)
---
Call two positive integers a **strong pair** when neither of them is more
than twice the other: writing them as `x <= y`, they are a strong pair
exactly when `y <= 2 * x`. So a number and its exact double are a strong
pair, but a number and anything bigger than its double are not.

Take two values `x` and `y` from the array `nums` that form a strong pair,
and work out their bitwise exclusive or, `x XOR y`. Both values may be the
same element of `nums`: every element makes a strong pair with itself,
which gives `0`. Return the largest `x XOR y` over all such choices.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 100`
