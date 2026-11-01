id: 1886
slug: determine-whether-matrix-can-be-obtained-by-rotation
title: Determine Whether Matrix Can Be Obtained By Rotation
difficulty: easy
tags: array, matrix
signature: bool findRotation(int[][] mat, int[][] target)
---
`mat` and `target` are square patterns of the same size, `n` rows of `n`
cells, and every cell holds `0` or `1`.

Imagine `mat` printed on a square card lying on a table. You may spin the
card flat on the table, a quarter turn (90 degrees) at a time, as often as
you like, and not spinning it at all is allowed too. You may **not** turn
the card over, so a mirror image of `mat` only counts if it happens to look
the same as one of its spins.

A clockwise quarter turn moves the cell in row `i` and column `j`
(counting from `0`) to row `j` and column `n - 1 - i`. Four quarter turns
bring the card back to where it started, so the card can only ever show
four patterns: `mat` itself, and `mat` turned by 90, 180 and 270 degrees.
Spinning the other way reaches the same four.

Return `true` if one of those four patterns matches `target` in every
cell, and `false` otherwise.

@examples

## Constraints

- `n == mat.length == target.length`
- `n == mat[i].length == target[i].length`
- `1 <= n <= 10`
- `mat[i][j]` and `target[i][j]` are `0` or `1`.
