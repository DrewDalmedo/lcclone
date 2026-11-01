id: 989
slug: add-to-array-form-of-integer
title: Add to Array-Form of Integer
difficulty: easy
tags: array, math
signature: int[] addToArrayForm(int[] num, int k)
---
Write a non-negative integer digit by digit, from its highest place down to
its units, and give each digit its own array element: the list you get is
the integer's **array form**. The array form of 6031 is `[6,0,3,1]`, and
that of zero is `[0]`. No other array form begins with a `0`.

`num` is the array form of some integer `N`, and `k` is an ordinary `int`.
Return the array form of `N + k`.

The answer follows the same rules: each element a single digit, highest
place first, and no `0` in front. It may hold more digits than `num`, when a
carry runs past the first digit of `N` or when `k` has more digits than
`N`.

`num` may be up to `10^4` digits long, so `N` won't fit in a 64-bit
integer, let alone an `int`.

@examples

## Constraints

- `1 <= num.length <= 10^4`
- `0 <= num[i] <= 9`
- `num[0] != 0`, unless `num` is `[0]`.
- `1 <= k <= 10^4`
