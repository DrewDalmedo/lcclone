id: 463
slug: island-perimeter
title: Island Perimeter
difficulty: easy
tags: depth-first-search, breadth-first-search, array, matrix
signature: int islandPerimeter(int[][] grid)
---
`grid` is a map of `rows` by `cols` square cells, each one unit on a side. A
cell holding `1` is land and a cell holding `0` is water, and everything
beyond the edges of the map is water as well.

The land cells make up one **island**: there is at least one, and you can
get from any land cell to any other by steps between cells that share a
side (a diagonal step doesn't count). No water is shut in by the island:
starting from any water cell, such steps through water alone lead to a cell
on the edge of the map.

Return the length of the island's coast: the number of cell sides that have
land on one side and water, or the outside of the map, on the other.

@examples

## Constraints

- `rows == grid.length`
- `cols == grid[i].length`
- `1 <= rows, cols <= 100`
- `grid[i][j]` is `0` or `1`.
- The land cells form exactly one island, and it shuts in no water.
