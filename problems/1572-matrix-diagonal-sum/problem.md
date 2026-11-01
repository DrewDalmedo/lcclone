id: 1572
slug: matrix-diagonal-sum
title: Matrix Diagonal Sum
difficulty: easy
tags: array, matrix
signature: int diagonalSum(int[][] mat)
---
`mat` is a square grid of `n` rows and `n` columns. Picture the two lines
that cross it from corner to corner: one runs from the top-left cell to the
bottom-right cell through the cells `(i, i)`, and the other from the
top-right cell to the bottom-left cell through the cells `(i, n - 1 - i)`.

Return the total of the numbers in the cells that these lines pass through.
When `n` is odd, both lines go through the middle cell; its number is added
only once.

@examples

## Constraints

- `n == mat.length == mat[i].length`
- `1 <= n <= 100`
- `1 <= mat[i][j] <= 100`
