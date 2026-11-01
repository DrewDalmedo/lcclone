id: 733
slug: flood-fill
title: Flood Fill
difficulty: easy
tags: depth-first-search, breadth-first-search, array, matrix
signature: int[][] floodFill(int[][] image, int sr, int sc, int color)
---
A floor is tiled in a grid of `m` rows and `n` columns, and `image[i][j]`
is the colour code of the tile in row `i`, column `j`. Two tiles are
**neighbours** when they share a side, so a tile has at most four of them;
tiles that meet only at a corner are not neighbours.

The tile in row `sr`, column `sc` is to be repainted with colour `color`,
and so is the rest of its **patch**: every tile you can reach from it by
moving from a tile to a neighbour, stepping only on tiles whose code is the
code the starting tile had before any repainting. Tiles with any other code,
`color` included, block the way.

Repaint every tile of the patch with `color` and leave all other tiles as
they are, including tiles with the starting code that lie in a different
patch. Return the floor afterwards, as `m` rows of `n` codes. When `color`
is the starting tile's code already, nothing changes.

@examples

## Constraints

- `m == image.length`
- `n == image[i].length`
- `1 <= m, n <= 50`
- `0 <= image[i][j], color < 2^16`
- `0 <= sr < m`
- `0 <= sc < n`
