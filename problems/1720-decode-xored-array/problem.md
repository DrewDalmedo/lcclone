id: 1720
slug: decode-xored-array
title: Decode XORed Array
difficulty: easy
tags: bit-manipulation, array
signature: int[] decode(int[] encoded, int first)
---
A list of `n` non-negative integers, `arr`, was stored in a compact delta
form. Its opening value was kept as it is, and you are given it as `first`.
Each later value was not stored itself; instead the list `encoded`, of
length `n - 1`, records how every value differs from the one before it, bit
by bit:

`encoded[i] = arr[i] XOR arr[i + 1]` for every `i` from `0` to `n - 2`.

Here `XOR` is the bitwise exclusive or (`^` in C and C++): a bit of the
result is `1` exactly when that bit is `1` in one of the two numbers but not
in both.

Rebuild `arr` and return it: all `n` values in their original order,
starting with `first`. Exactly one list matches `first` and `encoded`.

The values of `arr` are always below `2^17 = 131072`, but some of them may
be larger than `10^5`.

@examples

## Constraints

- `2 <= n <= 10^4`
- `encoded.length == n - 1`
- `0 <= encoded[i] <= 10^5`
- `0 <= first <= 10^5`
