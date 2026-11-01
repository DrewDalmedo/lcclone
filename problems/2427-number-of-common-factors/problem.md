id: 2427
slug: number-of-common-factors
title: Number of Common Factors
difficulty: easy
tags: math, enumeration, number-theory
signature: int commonFactors(int a, int b)
---
A rectangular floor measures `a` by `b` units. It is to be covered with
identical square tiles whose side `x` is a whole number of units, laid in
a grid with no gaps, no overlaps and no tile cut. Such a covering is
possible precisely when `a` and `b` are both whole multiples of `x`.

Return how many tile sides `x` make a covering possible, in other words
how many positive integers `x` divide both `a` and `b`. Tiles of side `1`
fit every floor; tiles as wide as the shorter side of the floor fit only
when that side divides the longer one.

@examples

## Constraints

- `1 <= a, b <= 1000`
