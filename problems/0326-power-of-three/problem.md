id: 326
slug: power-of-three
title: Power of Three
difficulty: easy
tags: recursion, math
signature: bool isPowerOfThree(int n)
---
Return `true` if `n` is a product of threes, `3 * 3 * ... * 3`, with some
number `k >= 0` of factors: that is, `n = 3^k`. Return `false` for every
other `n`.

With no factors at all, the product is `1`; so `n = 1`, which is `3^0`,
gets `true`. A product of threes is always positive, and no prime but `3`
divides it. The answer is therefore `false` for `0`, for every negative
`n` (`-3` and `-81` among them), and for a number like `6` or `63` that
some other prime divides.

@examples

## Constraints

- `-2^31 <= n <= 2^31 - 1`

## Follow-up

Dividing out the threes one at a time takes up to 19 steps. Could you
decide with straight-line code instead: the same few operations whatever
`n` is, without iterating and without a function that calls itself?
