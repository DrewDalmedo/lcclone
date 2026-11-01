id: 566
slug: reshape-the-matrix
title: Reshape the Matrix
difficulty: easy
tags: array, matrix, simulation
signature: int[][] matrixReshape(int[][] mat, int r, int c)
---
The grid `mat` is `m` rows tall and `n` columns wide. List its numbers in
**reading order**: the top row from left to right, then the row below it
from left to right, and so on down to the bottom row. That makes a list of
`m * n` numbers.

Write the same list into a new grid of `r` rows and `c` columns, again in
reading order: the first `c` numbers of the list become its top row, the
next `c` its second row, and so on. Return the new grid.

That can only be done when the new grid has exactly as many cells as `mat`,
which is when `r * c` equals `m * n`. When it doesn't, whether the new grid
would have too many cells or too few, return `mat` as it is.

Asking for `n` rows of `m` columns does not swap the rows and columns of
`mat`: the numbers still go in reading order, like for any other shape.

@examples

## Constraints

- `m == mat.length`
- `n == mat[i].length`
- `1 <= m, n <= 100`
- `-1000 <= mat[i][j] <= 1000`
- `1 <= r, c <= 300`
