id: 263
slug: ugly-number
title: Ugly Number
difficulty: easy
tags: math
signature: bool isUgly(int n)
---
Call an integer **ugly** when it can be made by multiplying together
nothing but 2s, 3s and 5s: that is, when it equals `2^a * 3^b * 5^c` for
some whole numbers `a`, `b` and `c`, each `0` or more.

Such a product is always positive. So `1` is ugly (take `a = b = c = 0`),
while `0` and every negative number are not. Neither is any number that a
prime larger than `5` divides, such as `7` or `22 = 2 * 11`.

Return `true` if `n` is ugly and `false` otherwise.

@examples

## Constraints

- `-2^31 <= n <= 2^31 - 1`
