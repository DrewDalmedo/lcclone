id: 1582
slug: special-positions-in-a-binary-matrix
title: Special Positions in a Binary Matrix
difficulty: easy
tags: array, matrix
signature: int numSpecial(int[][] mat)
---
`mat` is a grid of `m` rows and `n` columns, filled with `0`s and `1`s.
Call the cell `(i, j)`, in row `i` and column `j` (both counted from 0),
**special** if it holds a `1` that is alone twice over: no other cell of
row `i` holds a `1`, and no other cell of column `j` does either. A cell
holding `0` is never special.

Count the special cells of `mat` and return that count.

@examples

## Constraints

- `m == mat.length`
- `n == mat[i].length`
- `1 <= m, n <= 100`
- `mat[i][j]` is `0` or `1`.
