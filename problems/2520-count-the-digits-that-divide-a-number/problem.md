id: 2520
slug: count-the-digits-that-divide-a-number
title: Count the Digits That Divide a Number
difficulty: easy
tags: math
signature: int countDigits(int num)
---
Give each digit of `num` a mark: a tick when `num` is a multiple of that
digit `d`, that is, when `num % d == 0`, and a cross otherwise. Return the
number of ticks.

A digit that appears in several places is marked in each of them, and all
those marks are alike. Every digit is tested against all of `num`, never
against a piece of it. A `1` always earns a tick, and `num` has no digit
`0`, so no test ever divides by zero.

@examples

## Constraints

- `1 <= num <= 10^9`
- `num` does not contain the digit `0`.
