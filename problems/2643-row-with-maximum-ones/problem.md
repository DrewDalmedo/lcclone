id: 2643
slug: row-with-maximum-ones
title: Row With Maximum Ones
difficulty: easy
tags: array, matrix
signature: int[] rowAndMaximumOnes(int[][] mat)
---
`mat` has `m` rows, each a list of `n` zeros and ones mixed in any order.
Tally the `1`s of each row, then look for the row with the highest tally.

Return a pair `[r, c]`: `r` is the index of that row, counting from `0` at
the top, and `c` is how many `1`s it holds. If several rows share the
highest tally, `r` is the topmost of them, the one with the smallest index.
For example, a matrix made only of `0`s has every tally equal to `0`, so
the answer is `[0, 0]`.

@examples

## Constraints

- `m == mat.length`
- `n == mat[i].length`
- `1 <= m, n <= 100`
- Every `mat[i][j]` is `0` or `1`.
