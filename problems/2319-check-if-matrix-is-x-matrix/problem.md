id: 2319
slug: check-if-matrix-is-x-matrix
title: Check if Matrix Is X-Matrix
difficulty: easy
tags: array, matrix
signature: bool checkXMatrix(int[][] grid)
---
`grid` holds non-negative integers in `n` rows and `n` columns; number the
rows and the columns from `0`. Mark the cell in row `i` and column `j` when
`i == j` or when `i + j == n - 1`. The marked cells trace a big letter X
across the grid. When `n` is odd, the two strokes of the X share the middle
cell, and it is marked like any other.

The grid is an **X-matrix** when every marked cell holds a number other
than `0` and every unmarked cell holds `0`. Return `true` if `grid` is an
X-matrix, and `false` if some marked cell holds `0` or some unmarked cell
holds anything else.

@examples

## Constraints

- `n == grid.length == grid[i].length`
- `3 <= n <= 100`
- `0 <= grid[i][j] <= 10^5`
