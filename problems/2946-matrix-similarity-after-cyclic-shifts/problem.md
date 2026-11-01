id: 2946
slug: matrix-similarity-after-cyclic-shifts
title: Matrix Similarity After Cyclic Shifts
difficulty: easy
tags: array, math, matrix, simulation
signature: bool areSimilar(int[][] mat, int k)
---
Think of each row of the `m` by `n` matrix `mat` as a conveyor belt bent
into a ring: just past column `n - 1` the belt carries on at column `0`.
The rows are numbered from `0` at the top. A **tick** moves every belt by
one place, all at the same time:

- a row with an even number (`0`, `2`, `4`, ...) moves left: each value
  goes to the column one lower, and the value in column `0` comes round to
  column `n - 1`;
- a row with an odd number (`1`, `3`, `5`, ...) moves right: each value
  goes to the column one higher, and the value in column `n - 1` comes
  round to column `0`.

Let `k` ticks run, one after another. Return `true` if every position of
`mat` then holds the same value as it did before the first tick, and
`false` if at least one position holds a different value. `k` may be
larger than `n`.

@examples

## Constraints

- `m == mat.length`
- `n == mat[i].length`
- `1 <= m, n <= 25`
- `1 <= mat[i][j] <= 25`
- `1 <= k <= 50`
