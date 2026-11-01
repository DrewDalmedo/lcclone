id: 2729
slug: check-if-the-number-is-fascinating
title: Check if The Number is Fascinating
difficulty: easy
tags: hash-table, math
signature: bool isFascinating(int n)
---
Write the decimal digits of `n`, then those of `2 * n`, then those of
`3 * n`, all in one row with nothing between them. Each of the three
numbers is written in full, without leading zeros, so it adds as many
digits as it has: for `n = 410` the row is `4108201230`, ten digits long.

Call `n` **fascinating** when the row is `123456789` with its digits in
some order: nine digits long, no digit twice, and no `0`. Return `true` if
`n` is fascinating, and `false` otherwise.

@examples

## Constraints

- `100 <= n <= 999`
