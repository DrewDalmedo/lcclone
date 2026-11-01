id: 2965
slug: find-missing-and-repeated-values
title: Find Missing and Repeated Values
difficulty: easy
tags: array, hash-table, math, matrix
signature: int[] findMissingAndRepeatedValues(int[][] grid)
---
`grid` is a board of `n` rows and `n` columns. Its `n * n` squares were
meant to hold the whole numbers `1, 2, ..., n * n`, one number per square,
each number on exactly one square. One mistake was made: a number of that
range was written on two squares, so another number of the range was left
off the board, and every other number is on exactly one square as planned.

Return the two numbers involved in the mistake as an array, the one
written on two squares before the one that is on no square.

@examples

## Constraints

- `n == grid.length == grid[i].length`
- `2 <= n <= 50`
- `1 <= grid[i][j] <= n * n`
- Exactly one number of `1` to `n * n` is on no square, exactly one is on
  two squares, and every other one is on exactly one square.
