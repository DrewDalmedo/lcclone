id: 1351
slug: count-negative-numbers-in-a-sorted-matrix
title: Count Negative Numbers in a Sorted Matrix
difficulty: easy
tags: array, binary-search, matrix
signature: int countNegatives(int[][] grid)
---
`grid` is a table of integers with `m` rows and `n` columns, ordered so that
no number is ever bigger than the one before it: reading a row from left to
right, or a column from top to bottom, the values stay the same or go down.
Neighbouring cells may hold equal values.

Return how many cells of `grid` hold a **negative** number, one below `0`.
A cell holding `0` is not negative, so it is not counted.

@examples

## Constraints

- `m == grid.length`
- `n == grid[i].length`
- `1 <= m, n <= 100`
- `-100 <= grid[i][j] <= 100`
- Every row and every column of `grid` is in non-increasing order.

## Follow-up

Looking at every cell takes `O(m * n)` time. Can you use the ordering to
get the count in `O(m + n)` time?
