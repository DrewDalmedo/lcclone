id: 1137
slug: n-th-tribonacci-number
title: N-th Tribonacci Number
difficulty: easy
tags: memoization, math, dynamic-programming
signature: int tribonacci(int n)
---
The **tribonacci** sequence opens with three fixed terms: `0`, `1` and `1`.
From then on, each new term is the sum of the three terms just before it, so
the sequence carries on with `2`, `4`, `7`, `13`, `24` and so on.

Index the terms from zero: the opening `0` has index `0`, the two `1`s
have indices `1` and `2`, and the `2` after them, the first term found by
adding, has index `3`. Return the term whose index is `n`.

@examples

## Constraints

- `0 <= n <= 37`
- For every allowed `n`, the answer fits in a 32-bit signed `int`.
