id: 3345
slug: smallest-divisible-digit-product-i
title: Smallest Divisible Digit Product I
difficulty: easy
tags: math, enumeration
signature: int smallestNumber(int n, int t)
---
A positive integer's **digit product** is the product of its decimal
digits, written without leading zeros. A one-digit number is its own digit
product, `64` has digit product `6 * 4 = 24`, and any number that contains
the digit `0`, such as `50` or `100`, has digit product `0`.

Count upward from `n`: `n`, `n + 1`, `n + 2`, and so on. Return the first
number you reach whose digit product is a multiple of `t`. That may be `n`
itself.

`0` is a multiple of every `t`, so a number with a `0` digit always
qualifies.

@examples

## Constraints

- `1 <= n <= 100`
- `1 <= t <= 10`
