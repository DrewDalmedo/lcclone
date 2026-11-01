id: 3417
slug: zigzag-grid-traversal-with-skip
title: Zigzag Grid Traversal With Skip
difficulty: easy
tags: array, matrix, simulation
signature: int[] zigzagTraversal(int[][] grid)
---
A gardener tends a field laid out as `grid`: `R` rows of `C` plants each,
where `grid[r][c]` is the weight of the plant in row `r`, column `c`. She
walks through the whole field once, along a snake-shaped path:

- row `0` from its leftmost plant (column `0`) to its rightmost
  (column `C - 1`),
- then down to row `1`, which she walks from right to left,
- then down to row `2`, walked left to right again,

and so on, changing direction at every row, until she has passed every
plant of the last row.

On the way she picks the first plant she passes, leaves the second, picks
the third, and keeps alternating like this, one plant at a time, for the
whole walk. Going down to a new row does **not** start the pattern over:
the first plant of a row is picked exactly when the last plant of the row
before it was left.

Return the weights of the plants she picks, in the order she picks them.

@examples

## Constraints

- `R == grid.length`
- `C == grid[r].length`
- `2 <= R, C <= 50`
- `1 <= grid[r][c] <= 2500`
