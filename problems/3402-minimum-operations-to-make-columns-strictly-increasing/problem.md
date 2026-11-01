id: 3402
slug: minimum-operations-to-make-columns-strictly-increasing
title: Minimum Operations to Make Columns Strictly Increasing
difficulty: easy
tags: greedy, array, matrix
signature: int minimumOperations(int[][] grid)
---
`grid` is a table of non-negative integers with `m` rows and `n` columns.
Read a column from the top (row `0`) down to the bottom (row `m - 1`). The
column **rises strictly** when each number in it is greater than the number
directly above it. Two equal numbers one above the other break the rise.

One operation adds `1` to the number in a single cell of your choice. That
is the only change allowed: no number is ever lowered, and cells never move.

Return the fewest operations after which every column of `grid` rises
strictly. Columns are judged one at a time, so how the numbers of a row
compare with each other doesn't matter. With a single row there is nothing
to fix, and the answer is `0`.

@examples

## Constraints

- `m == grid.length`
- `n == grid[i].length`
- `1 <= m, n <= 50`
- `0 <= grid[i][j] < 2500`
