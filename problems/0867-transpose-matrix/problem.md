id: 867
slug: transpose-matrix
title: Transpose Matrix
difficulty: easy
tags: array, matrix, simulation
signature: int[][] transpose(int[][] matrix)
---
`matrix` has `m` rows of `n` integers each. Return its **transpose**: the
matrix with `n` rows of `m` integers, where the number in row `i` and column
`j` of `matrix` moves to row `j` and column `i`.

In other words, the first column of `matrix` becomes the first row of the
answer, the second column the second row, and so on.

@examples

## Constraints

- `m == matrix.length`
- `n == matrix[i].length`
- `1 <= m, n <= 1000`
- `1 <= m * n <= 10^5`
- `-10^9 <= matrix[i][j] <= 10^9`
