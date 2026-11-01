id: 1337
slug: the-k-weakest-rows-in-a-matrix
title: The K Weakest Rows in a Matrix
difficulty: easy
tags: array, binary-search, matrix, sorting, heap-priority-queue
signature: int[] kWeakestRows(int[][] mat, int k)
---
Every cell of the `m` by `n` matrix `mat` holds a `0` or a `1`, and each
row is filled from the left like a gauge: it begins with all of its `1`s,
and everything after its first `0` is `0` as well. A row may be all `1`s
or all `0`s. The **strength** of a row is how many `1`s it holds, and the
rows are numbered from `0` (the top row) to `m - 1`.

Rank the rows with the weakest first: by increasing strength, and, when
two rows are equally strong, the one with the smaller number first.
Return the numbers of the first `k` rows of that ranking, in ranking
order.

@examples

## Constraints

- `m == mat.length`
- `n == mat[i].length`
- `2 <= m, n <= 100`
- `1 <= k <= m`
- Every `mat[i][j]` is `0` or `1`, and in each row no `1` comes after a
  `0`.
