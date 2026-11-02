id: 3697
slug: compute-decimal-representation
title: Compute Decimal Representation
difficulty: easy
tags: array, math
signature: int[] decimalRepresentation(int n)
---
Call a positive integer **round** when its decimal form has exactly one
non-zero digit, the leading one, with only zeros after it. So `4`, `70`,
`300` and `9000` are round; `45` has two non-zero digits and `2020` has a
non-zero digit after a zero, so neither is. Every round number is
`d * 10^e` for a digit `d` from `1` to `9` and some `e >= 0`.

Write `n` in **expanded form**, as a sum of round numbers taken straight from
its digits: each non-zero digit `d` of `n` gives the round number made of `d`
followed by as many zeros as there are digits to its right in `n`. A digit
`0` gives nothing. For instance, `4027` expands to `4000 + 20 + 7`.

Return these round numbers in an array, from the largest to the smallest,
which is the order in which their digits appear in `n`, read from left to
right. The array has one element per non-zero digit of `n`, so it is never
empty and never holds a `0`.

No sum of fewer round numbers can equal `n`, so the expanded form is as
short as possible. Other sums of the same length may exist (`15` is also
`8 + 7`), but only the expanded form is accepted.

@examples

## Constraints

- `1 <= n <= 10^9`
