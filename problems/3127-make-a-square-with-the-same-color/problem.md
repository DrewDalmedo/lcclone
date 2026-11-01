id: 3127
slug: make-a-square-with-the-same-color
title: Make a Square with the Same Color
difficulty: easy
tags: array, enumeration, matrix
signature: bool canMakeSquare(char[][] grid)
---
`grid` describes a board of 3 rows and 3 columns of tiles. Each tile is
black or white: `grid[r][c]` is `'B'` for a black tile and `'W'` for a
white one.

A **block** is a group of four tiles that lie in two neighbouring rows and
two neighbouring columns, so it forms a 2 x 2 square on the board. The
board has exactly four blocks: rows 0 and 1 or rows 1 and 2, crossed with
columns 0 and 1 or columns 1 and 2. All four contain the centre tile.

You may repaint one tile, turning black into white or white into black, or
leave the board as it is. Return `true` if you can end up with at least one
block whose four tiles all have the same color, black or white, and
`false` if you can't.

@examples

## Constraints

- `grid.length == 3`
- `grid[i].length == 3`
- Every `grid[i][j]` is `'B'` or `'W'`.
