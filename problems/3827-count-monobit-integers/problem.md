id: 3827
slug: count-monobit-integers
title: Count Monobit Integers
difficulty: easy
tags: bit-manipulation, enumeration
signature: int countMonobit(int n)
---
Say that a non-negative integer is **monobit** when its binary form is one
digit repeated, so that it uses only `0`s or only `1`s.

The binary form is the shortest one, with no leading zeros. So `6` is
`110`, not `0110`, and is not monobit; `8` is `1000` and is not monobit
either; `7` is `111` and is monobit. Zero is written as the single digit
`0`, so it is monobit as well.

Return how many of the integers `0, 1, 2, ..., n` are monobit, counting
both `0` and `n` themselves.

@examples

## Constraints

- `0 <= n <= 1000`
