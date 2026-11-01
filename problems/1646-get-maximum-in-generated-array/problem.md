id: 1646
slug: get-maximum-in-generated-array
title: Get Maximum in Generated Array
difficulty: easy
tags: array, simulation
signature: int getMaximumGenerated(int n)
---
Fill in a list of numbers `a[0]`, `a[1]`, ..., `a[n]`, one for every index
from `0` to `n`, so `n + 1` numbers in all. Work from left to right:

- `a[0]` is `0`, and `a[1]` is `1` (when `n >= 1`; for `n = 0` the list
  holds just `a[0]`).
- Every later index `j`, from `2` up to `n`, takes its number from the
  middle of the indices before it. If `j` is even, `a[j]` is a copy of
  `a[j / 2]`. If `j` is odd, half of `j` lies between the two indices
  `(j - 1) / 2` and `(j + 1) / 2`, and `a[j]` is the sum of the numbers at
  those two indices.

Both rules only read numbers that are already in the list. Return the
largest number in the finished list, counting both ends, `a[0]` and `a[n]`.

@examples

## Constraints

- `0 <= n <= 100`
