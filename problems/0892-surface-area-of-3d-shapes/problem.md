id: 892
slug: surface-area-of-3d-shapes
title: Surface Area of 3D Shapes
difficulty: easy
tags: geometry, array, math, matrix
signature: int surfaceArea(int[][] grid)
---
A square board is divided into `n` rows and `n` columns of cells, every
cell a 1 by 1 square. On the cell in row `i` and column `j` stands a stack
of `grid[i][j]` unit cubes, piled straight up from the board. A `0` leaves
the cell empty.

You are going to paint the outside of everything built. Each face of each
cube takes one unit square of paint, unless it is pressed flat against
another cube. That happens in two ways:

- between two cubes of the same stack, one resting on the other;
- between two cubes at the same height in neighbouring cells, cells that
  share a side. Cells that meet only at a corner hide nothing from each
  other.

The board hides nothing: the bottom face of every stack is painted too.

Return the number of unit squares you paint. If the cubes make up several
separate pieces, paint them all. A side of a stack that faces an empty
cell is painted even when that empty cell has stacks around it on all
four sides.

@examples

## Constraints

- `n == grid.length == grid[i].length`
- `1 <= n <= 50`
- `0 <= grid[i][j] <= 50`
