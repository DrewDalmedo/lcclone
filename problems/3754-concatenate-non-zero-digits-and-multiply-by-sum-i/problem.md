id: 3754
slug: concatenate-non-zero-digits-and-multiply-by-sum-i
title: Concatenate Non-Zero Digits and Multiply by Sum I
difficulty: easy
tags: math
signature: long sumAndMultiply(int n)
---
Strike out every `0` from the decimal form of `n`, and read the digits that
remain, from left to right, as a number `x`. When nothing remains, which
happens only for `n = 0`, take `x = 0`.

Multiply `x` by the total of its own digits, and return the product.

@examples

## Constraints

- `0 <= n <= 10^9`
