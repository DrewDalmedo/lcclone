id: 1260
slug: shift-2d-grid
title: Shift 2D Grid
difficulty: easy
tags: array, matrix, simulation
signature: int[][] shiftGrid(int[][] grid, int k)
---
`grid` has `m` rows of `n` integers. Chain its cells into one closed loop
by giving each cell a **successor**:

- for a cell that is not the last of its row, the cell just to its right;
- for the last cell of any row but the bottom one, the first cell of the
  next row down;
- for the bottom-right cell, the top-left cell, which closes the loop.

A **shift** moves every value, all at the same moment, from its cell into
that cell's successor. Return the grid after `k` shifts, still `m` rows of
`n` integers.

After `m * n` shifts every value is back in its starting cell, and `k`
may be larger than `m * n`.

@examples

## Constraints

- `m == grid.length`
- `n == grid[i].length`
- `1 <= m, n <= 50`
- `-1000 <= grid[i][j] <= 1000`
- `0 <= k <= 100`
