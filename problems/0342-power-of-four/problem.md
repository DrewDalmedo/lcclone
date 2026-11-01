id: 342
slug: power-of-four
title: Power of Four
difficulty: easy
tags: bit-manipulation, recursion, math
signature: bool isPowerOfFour(int n)
---
Write `n` in base 4, where the digits run from `0` to `3` and each place is
worth four times the place to its right. Return `true` if you get a `1`
followed only by zeros, or by nothing at all: `1`, `10`, `100`, `1000` and
so on. Return `false` otherwise, and always for zero and negative `n`.

The numbers that look like this in base 4 are the **powers of four**,
`4^k` for a whole number `k >= 0`; in base 10 they begin `1`, `4`, `16`,
`64`. Only every other power of two is among them: `2`, `8` and `32`,
whose exponent of two is odd, are `2`, `20` and `200` in base 4.

@examples

## Constraints

- `-2^31 <= n <= 2^31 - 1`

## Follow-up

A loop, or recursion, can divide `n` by `4` until its last base-4 digit
isn't `0`. Can you get the answer without either?
