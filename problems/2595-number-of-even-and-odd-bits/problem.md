id: 2595
slug: number-of-even-and-odd-bits
title: Number of Even and Odd Bits
difficulty: easy
tags: bit-manipulation
signature: int[] evenOddBit(int n)
---
A positive integer can be written as a sum of distinct powers of two in
exactly one way. Write `n` like that and look at the exponents that occur:
`13 = 2^3 + 2^2 + 2^0` uses the exponents `3`, `2` and `0`. In binary,
these exponents are the positions of the `1` bits when positions are
counted from the least significant bit, which is at position `0`.

Return an array of two counts: first how many of those exponents are
even, then how many are odd. `0` counts as even.

Below, every example shows `n` split into its powers of two.

@examples

## Constraints

- `1 <= n <= 1000`
