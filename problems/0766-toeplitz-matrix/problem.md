id: 766
slug: toeplitz-matrix
title: Toeplitz Matrix
difficulty: easy
tags: array, matrix
signature: bool isToeplitzMatrix(int[][] matrix)
---
Pick a cell in the top row or the left column of `matrix`, and walk from it
down and to the right, one row and one column per step, until the next step
would leave the matrix. The cells you pass, the first one included, make up
a **slant**. Every cell lies on exactly one slant; a matrix of `m` rows and
`n` columns has `m + n - 1` of them, and the slants that start in its top
right and bottom left corners are a single cell long.

`matrix` is a **Toeplitz** matrix when each of its slants holds one number
throughout. Return `true` if it is, and `false` if some slant holds two
different numbers. Lines running down and to the left play no part.

@examples

## Constraints

- `m == matrix.length`
- `n == matrix[i].length`
- `1 <= m, n <= 20`
- `0 <= matrix[i][j] <= 99`

## Follow-up

Say `matrix` is far too large to hold in memory, and you can only read it
row by row from the top. How little of the rows already read do you need
to keep? And how would you check it if memory can't hold even one complete
row?
