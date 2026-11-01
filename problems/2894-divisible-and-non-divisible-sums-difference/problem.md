id: 2894
slug: divisible-and-non-divisible-sums-difference
title: Divisible and Non-divisible Sums Difference
difficulty: easy
tags: math
signature: int differenceOfSums(int n, int m)
---
Write the numbers `1, 2, ..., n` in a row, from `1` up to and including
`n`, and put a sign in front of each one: a minus sign in front of every
multiple of `m` (a number that `m` divides with no remainder), and a plus
sign in front of every other number. Return the value of the expression
this gives.

Put another way: add up the numbers from `1` to `n` that are not multiples
of `m`, then subtract the sum of the ones that are. The result is negative
when the multiples outweigh the rest (always so when `m = 1`, since every
number is then a multiple), and when `m` is larger than `n` nothing is
subtracted at all.

@examples

## Constraints

- `1 <= n <= 1000`
- `1 <= m <= 1000`
