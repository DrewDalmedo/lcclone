id: 1952
slug: three-divisors
title: Three Divisors
difficulty: easy
tags: math, enumeration, number-theory
signature: bool isThree(int n)
---
A marching band of `n` players forms up as a rectangle: some number of
rows `r`, each holding the same number of players, with no gaps and nobody
left over. A row count `r` works exactly when it divides `n` with no
remainder. One row of all `n` players and `n` rows of one player each
always work.

Return `true` if exactly three row counts work for the band, and `false`
otherwise. In other words, decide whether `n` has exactly three positive
divisors, counting `1` and `n` itself.

@examples

## Constraints

- `1 <= n <= 10^4`
