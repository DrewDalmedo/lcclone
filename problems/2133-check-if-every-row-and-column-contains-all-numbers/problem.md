id: 2133
slug: check-if-every-row-and-column-contains-all-numbers
title: Check if Every Row and Column Contains All Numbers
difficulty: easy
tags: array, hash-table, matrix
signature: bool checkValid(int[][] matrix)
---
`matrix` is a square grid of `n` rows and `n` columns, and every number in
it lies between `1` and `n`. Call a row or a column **complete** when each
of the integers `1, 2, ..., n` shows up somewhere in it.

Return `true` if all `n` rows and all `n` columns are complete, and `false`
if at least one of them is not.

A row or a column has exactly `n` cells, so a complete one holds each of
`1` to `n` exactly once. It is incomplete exactly when some number appears
in it more than once.

@examples

## Constraints

- `n == matrix.length == matrix[i].length`
- `1 <= n <= 100`
- `1 <= matrix[i][j] <= n`
