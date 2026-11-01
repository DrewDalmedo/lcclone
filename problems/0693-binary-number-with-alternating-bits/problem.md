id: 693
slug: binary-number-with-alternating-bits
title: Binary Number with Alternating Bits
difficulty: easy
tags: bit-manipulation
signature: bool hasAlternatingBits(int n)
---
Halve the positive integer `n` over and over, rounding down, until it
reaches `0`. Every halving leaves a remainder, `0` or `1`, and these
remainders, in the order they come out, are the binary digits of `n` from
the lowest one up. The last of them is always a `1`.

Return `true` if each remainder differs from the one that came out just
before it, so that `0`s and `1`s take turns all the way, and `false` if
some two remainders in a row are equal.

The halving stops at `0`, so the unused high bits of a 32-bit `int`, which
are all zeros, never come out as remainders and can't form an equal pair.
For `n = 1` there is a single remainder and no pair at all, so the answer
is `true`.

Each example's explanation first rewrites `n` from decimal into binary.

@examples

## Constraints

- `1 <= n <= 2^31 - 1`
