id: 231
slug: power-of-two
title: Power of Two
difficulty: easy
tags: bit-manipulation, recursion, math
signature: bool isPowerOfTwo(int n)
---
The powers of two are `1`, `2`, `4`, `8`, `16` and so on, each double the
one before: the numbers `2^k` for a whole number `k >= 0`. Return `true` if
`n` is one of them, and `false` otherwise.

`1 = 2^0` counts. Zero and negative numbers never do, since doubling `1`
any number of times can't reach them.

@examples

## Constraints

- `-2^31 <= n <= 2^31 - 1`

## Follow-up

Can you answer with a fixed handful of operations, so that the function
has no loop and never calls itself?
