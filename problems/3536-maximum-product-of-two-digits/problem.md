id: 3536
slug: maximum-product-of-two-digits
title: Maximum Product of Two Digits
difficulty: easy
tags: math, sorting
signature: int maxProduct(int n)
---
Write each decimal digit of `n` on a card of its own, one card per digit:
`3077` gives four cards, showing `3`, `0`, `7` and `7`. Pick any two of the
cards and multiply the digits on them. Return the largest product two cards
can give.

The two cards must be different cards, so squaring a digit takes two cards
that both show it: `3077` allows `7 * 7 = 49`, but `3071`, where only one
card shows `7`, gets no further than `7 * 3 = 21`.

@examples

## Constraints

- `10 <= n <= 10^9`
