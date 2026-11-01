id: 3954
slug: sum-of-compatible-numbers-in-range-i
title: Sum of Compatible Numbers in Range I
difficulty: easy
tags: bit-manipulation, dynamic-programming, enumeration
signature: int sumOfGoodIntegers(int n, int k)
---
Think of a positive integer as the set of bit positions where its binary
form has a `1`: `13`, which is `1101` in binary, is the set of positions
`0`, `2` and `3`. Two integers **overlap** when their sets share a
position, which is when their bitwise AND is not zero.

An integer `x` is **compatible** with `n` when all three of these hold:

- `x >= 1`: zero and negative numbers never count;
- `x` lies in the **window** around `n`: `n - k <= x <= n + k`, with both
  ends included, so `x` is at most `k` away from `n`;
- `x` and `n` don't overlap: `(n & x) == 0`.

Return the sum of all the integers that are compatible with `n`, or `0` if
there are none. When `k` is large the window reaches below `1`; only its
positive part is searched. `n` itself never qualifies, since it overlaps
itself.

@examples

## Constraints

- `1 <= n <= 100`
- `1 <= k <= 100`
