id: 2485
slug: find-the-pivot-integer
title: Find the Pivot Integer
difficulty: easy
tags: math, prefix-sum
signature: int pivotInteger(int n)
---
A ladder has `n` rungs, numbered from `1` at the bottom to `n` at the top.
Standing on rung `x`, add up the numbers of the rungs from the bottom up
to where you stand, `1 + 2 + ... + x`, and separately the numbers of the
rungs from where you stand up to the top, `x + (x + 1) + ... + n`. The rung
you stand on goes into both totals. Rung `x` is the **balance rung** when
the two totals are equal.

Return the number of the balance rung, or `-1` if the ladder has none. No
ladder has more than one.

@examples

## Constraints

- `1 <= n <= 1000`
