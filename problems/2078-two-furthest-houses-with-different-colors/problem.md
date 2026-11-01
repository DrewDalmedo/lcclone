id: 2078
slug: two-furthest-houses-with-different-colors
title: Two Furthest Houses With Different Colors
difficulty: easy
tags: greedy, array
signature: int maxDistance(int[] colors)
---
A street has `n` houses in a row at positions `0` to `n - 1`, and the house
at position `i` is painted in color number `colors[i]`. Every number is a
real color, `0` included, and two houses look alike exactly when their
numbers are equal.

You want to pick two houses that do **not** look alike and that stand as
far apart as possible, where houses at positions `i` and `j` are `|i - j|`
apart. Return how far apart such a pair can be. At least two houses differ
in color, so a pair can always be picked.

@examples

## Constraints

- `n == colors.length`
- `2 <= n <= 100`
- `0 <= colors[i] <= 100`
- At least two houses have different colors.
