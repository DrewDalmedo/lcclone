id: 3033
slug: modify-the-matrix
title: Modify the Matrix
difficulty: easy
tags: array, matrix
signature: int[][] modifiedMatrix(int[][] matrix)
---
`matrix` is a grid of `m` rows and `n` columns. Its cells hold numbers from
`0` to `100`, except for some that hold `-1`: those are **blanks**, cells
whose value has gone missing.

Fill in each blank with the largest number found anywhere in the same
column, and return the grid you get. It has the shape of `matrix`, and every
cell that was not a blank keeps its number, `0` included. Every column has
at least one cell that is not a blank, so its largest number is always
one of `0` to `100`; the blanks themselves never decide it.

@examples

## Constraints

- `m == matrix.length`
- `n == matrix[i].length`
- `2 <= m, n <= 50`
- `-1 <= matrix[i][j] <= 100`
- Each column of `matrix` has at least one number that is not `-1`.
