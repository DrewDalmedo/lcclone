id: 461
slug: hamming-distance
title: Hamming Distance
difficulty: easy
tags: bit-manipulation
signature: int hammingDistance(int x, int y)
---
Number the bits of a non-negative integer from the low end: bit `i` is the
binary digit worth `2^i`, and every bit above the number's highest `1` is
`0`. The **Hamming distance** of `x` and `y` is how many indices `i` have
bit `i` of `x` different from bit `i` of `y`, one of them `0` and the other
`1`.

Return the Hamming distance of `x` and `y`. Both are below `2^31`, so only
bits `0` to `30` can differ, and the answer is at most `31`.

In the examples `x` and `y` are written in base 10; each explanation shows
them in binary.

@examples

## Constraints

- `0 <= x, y <= 2^31 - 1`
