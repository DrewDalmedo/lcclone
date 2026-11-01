id: 2373
slug: largest-local-values-in-a-matrix
title: Largest Local Values in a Matrix
difficulty: easy
tags: array, matrix
signature: int[][] largestLocal(int[][] grid)
---
`grid` is a square map of `n` by `n` plots of land: `grid[r][c]` is the
height of the plot in row `r` and column `c`, counting from `0`.

A **patch** is a 3 by 3 block of plots that lies wholly inside the map. The
patch at `(i, j)` has `(i, j)` as its top-left plot: it covers rows `i`,
`i + 1`, `i + 2` and columns `j`, `j + 1`, `j + 2`. So `i` and `j` each run
from `0` to `n - 3`, and there are no smaller patches cut off at the edges.

Return the `(n - 2)` by `(n - 2)` matrix `peaks` in which `peaks[i][j]` is
the greatest height in the patch at `(i, j)`. Row `i` of `peaks` holds,
from left to right, the peaks of the patches whose top row is row `i` of
`grid`.

@examples

## Constraints

- `n == grid.length == grid[i].length`
- `3 <= n <= 100`
- `1 <= grid[i][j] <= 100`
