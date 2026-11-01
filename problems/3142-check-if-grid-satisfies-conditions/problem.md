id: 3142
slug: check-if-grid-satisfies-conditions
title: Check if Grid Satisfies Conditions
difficulty: easy
tags: array, matrix
signature: bool satisfiesConditions(int[][] grid)
---
Picture `grid`, which has `m` rows and `n` columns, as a wall of tiles, each
number being the colour of one tile. The wall is **neatly striped** when
every tile passes two tests:

- the tile straight below it, if there is one, has the **same** colour;
- the tile straight to its right, if there is one, has a **different**
  colour.

A tile in the bottom row has nothing below it, and a tile in the rightmost
column has nothing to its right; for such a tile the missing test is simply
skipped, not failed. Return `true` if the wall is neatly striped and
`false` otherwise.

Taken together, the tests ask for each column to keep one colour from top to
bottom, and for any two columns standing side by side to have different
colours. Columns that don't touch may well share a colour.

@examples

## Constraints

- `m == grid.length`
- `n == grid[i].length`
- `1 <= m, n <= 10`
- `0 <= grid[i][j] <= 9`
