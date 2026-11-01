id: 883
slug: projection-area-of-3d-shapes
title: Projection Area of 3D Shapes
difficulty: easy
tags: geometry, array, math, matrix
signature: int projectionArea(int[][] grid)
---
A floor is tiled in `n` rows of `n` square tiles, and someone builds on it
with identical cube-shaped blocks, each exactly one tile wide. The tile in
row `i`, column `j` gets `grid[i][j]` blocks piled up on it, each resting
squarely on the tile or on the block beneath; call such a pile a **tower**.
Where the value is `0`, the tile stays uncovered.

Now look at the towers from far away in three directions, each straight
along an edge of the blocks:

- from **above**, looking down at the floor;
- along the **rows**, so that the blocks of each row line up one behind
  another;
- along the **columns**, so that the blocks of each column do.

Each view is a flat picture made of squares the size of one face of a
block. A square is in the picture when at least one block lies behind it,
and it counts once however many do. Return the total number of squares in
the three pictures.

@examples

## Constraints

- `n == grid.length == grid[i].length`
- `1 <= n <= 50`
- `0 <= grid[i][j] <= 50`
