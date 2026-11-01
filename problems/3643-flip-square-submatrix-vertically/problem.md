id: 3643
slug: flip-square-submatrix-vertically
title: Flip Square Submatrix Vertically
difficulty: easy
tags: array, two-pointers, matrix
signature: int[][] reverseSubmatrix(int[][] grid, int x, int y, int k)
---
`grid` is a wall of tiles laid out in `m` rows and `n` columns, both
numbered from `0`, with a number on every tile. Mark out the square block
of `k` by `k` tiles whose top-left tile sits in **row `x`** and
**column `y`**: it covers rows `x` to `x + k - 1` and columns `y` to
`y + k - 1`.

Turn that block upside down, top to bottom. The block's top row of tiles
trades places with its bottom row, its second row with its second-to-last,
and so on; when `k` is odd, the middle row stays put. Each tile keeps its
column, so within a row the left-to-right order doesn't change. In other
words, for each offset `r` from `0` to `k - 1`, the tiles of row `x + r`
in columns `y` to `y + k - 1` take the numbers that row `x + k - 1 - r`
had in those columns.

Every tile outside the block keeps its number, including those in rows
`x` to `x + k - 1` that lie to the left or right of the block. Return the
whole wall after the change, as `m` rows of `n` numbers.

@examples

## Constraints

- `m == grid.length`
- `n == grid[i].length`
- `1 <= m, n <= 50`
- `1 <= grid[i][j] <= 100`
- `0 <= x < m`
- `0 <= y < n`
- `1 <= k <= min(m - x, n - y)`
