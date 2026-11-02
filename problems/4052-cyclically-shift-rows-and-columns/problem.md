id: 4052
slug: cyclically-shift-rows-and-columns
title: Cyclically Shift Rows and Columns
difficulty: easy
tags: array, matrix, simulation
signature: int[][] cyclicShift(int n, int[][] grid, int[] rowShift, int[] colShift)
---
`grid` is a square board of numbered tiles, `n` rows by `n` columns, with
rows numbered from `0` at the top and columns from `0` at the left. A whole
row or a whole column of tiles can slide at once, and the board wraps
around: a tile pushed off one end of its row or column comes back in at the
other end.

The board is scrambled in two rounds, the second one starting from the
grid the first one leaves.

1. **Rows.** Row `i` slides to the left by `rowShift[i]` places. Sliding
   left by one place moves each tile of the row into the column on its
   left, and the tile from column `0` comes back in at the right end, in
   column `n - 1`. So after this round, the tile in row `i` and column `j`
   is the one that started in column `(j + rowShift[i]) mod n` of row `i`.
2. **Columns.** Column `j` slides up by `colShift[j]` places. Sliding up by
   one place moves each tile of the column into the row above it, and the
   tile from row `0` comes back in at the bottom, in row `n - 1`. So after
   this round, the tile in row `i` and column `j` is the one that was in row
   `(i + colShift[j]) mod n` of column `j` when the first round ended.

`colShift[j]` belongs to column number `j` of the grid after the first
round, whichever tiles stand in it then. Return the grid after both rounds.
Two tiles may carry the same number.

@examples

## Constraints

- `1 <= n <= 10`
- `grid.length == n` and `grid[i].length == n`
- `1 <= grid[i][j] <= 100`
- `rowShift.length == colShift.length == n`
- `0 <= rowShift[i] < n`
- `0 <= colShift[j] < n`
