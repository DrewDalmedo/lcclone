id: 1281
slug: subtract-the-product-and-sum-of-digits-of-an-integer
title: Subtract the Product and Sum of Digits of an Integer
difficulty: easy
tags: math
signature: int subtractProductAndSum(int n)
---
Write the positive integer `n` in decimal and look at its digits. Multiply
all of them together to get `P`, and add all of them up to get `S`. Return
`P - S`, which may be negative or zero.

Every digit takes part, zeros included: a single `0` among the digits makes
`P` zero, and the answer is then `-S`. When `n` has only one digit, `P` and
`S` are both that digit, so the answer is `0`.

@examples

## Constraints

- `1 <= n <= 10^5`
