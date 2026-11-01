id: 3622
slug: check-divisibility-by-digit-sum-and-product
title: Check Divisibility by Digit Sum and Product
difficulty: easy
tags: math
signature: bool checkDivisibility(int n)
---
Take the decimal digits of `n` and combine them twice: adding them all up
gives their **sum**, and multiplying them all together gives their
**product**. Let `t` be the sum plus the product.

Return `true` if dividing `n` by `t` gives a whole number, and `false` if
it leaves a remainder.

No digit is skipped: a `0` counts too, and it makes the product `0`. For
example, `305` has sum `8` and product `3 * 0 * 5 = 0`, so its `t` is
`8`. Since `n` is positive, it has a nonzero digit, so `t` is never `0`.

@examples

## Constraints

- `1 <= n <= 10^6`
