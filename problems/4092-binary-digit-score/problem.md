id: 4092
slug: binary-digit-score
title: Binary Digit Score
difficulty: easy
tags:
signature: int calculateScore(int a, int b)
---
Give each of `a` and `b` a **weight**: the count of `1` digits in its
base-2 form. Let `p` be the weight of `a`, and `q` the weight of `b`. Only
the ones count, not the length: `64` is `1000000` in binary, seven digits
long, but its weight is `1`.

Comparing the two weights decides the score of the pair:

- if `p >= q`, ties included, the score is the product `p * q`;
- if `p < q`, the score is the quotient `q / p`, rounded down to a whole
  number.

Return the score. The pair is ordered, so `(a, b)` and `(b, a)` can fall
under different rules. As `a` and `b` are at least `1`, `p` is at least
`1` and the division is always defined.

@examples

## Constraints

- `1 <= a <= 100`
- `1 <= b <= 100`
