id: 3945
slug: digit-frequency-score
title: Digit Frequency Score
difficulty: easy
tags: hash-table, math
signature: int digitFrequencyScore(int n)
---
Write the positive integer `n` in decimal and make a tally of its digits:
for each of the ten digits `0` to `9`, how many times it occurs in `n`.

A digit `d` that occurs `c` times scores `d * c` points, and a digit that
does not occur scores nothing. Return the total of the points scored by all
ten digits.

Every occurrence is counted, so a digit that is repeated scores once for
each copy. A `0` scores `0` points however often it occurs.

@examples

## Constraints

- `1 <= n <= 10^9`
