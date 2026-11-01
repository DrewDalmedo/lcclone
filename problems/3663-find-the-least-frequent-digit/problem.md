id: 3663
slug: find-the-least-frequent-digit
title: Find The Least Frequent Digit
difficulty: easy
tags: array, hash-table, math, counting
signature: int getLeastFrequentDigit(int n)
---
Write the positive integer `n` in decimal, with no leading zeros, and look
at the digits that occur in it. Each digit that occurs has a **tally**:
how many times it is written. A digit that does not occur in `n` has no
tally and is left out entirely, so the answer is always a digit that `n`
contains.

Return the digit whose tally is lowest. A tie on the lowest tally goes
to the digit of least value, regardless of the order in which the digits
appear in `n`. So if `n` contains a `0` and no digit has a lower tally
than that `0`, the answer is `0`.

@examples

## Constraints

- `1 <= n <= 2^31 - 1`
