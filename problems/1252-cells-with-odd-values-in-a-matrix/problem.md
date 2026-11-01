id: 1252
slug: cells-with-odd-values-in-a-matrix
title: Cells with Odd Values in a Matrix
difficulty: easy
tags: array, math, simulation
signature: int oddCells(int m, int n, int[][] indices)
---
Picture a board of `m` rows and `n` columns in which every square holds
`0`. Both rows and columns are numbered from `0`.

Each pair `[r, c]` of `indices` is one step, made of two parts: add `1` to
all `n` squares of row `r`, and add `1` to all `m` squares of column `c`.
The square where that row and that column meet belongs to both, so the
step raises it by `2`, while every other square of the row or the column
goes up by `1`. A pair may occur in `indices` more than once, and each
occurrence is a step of its own.

Carry out every step, then return how many squares of the board hold an
odd number.

@examples

## Constraints

- `1 <= m, n <= 50`
- `1 <= indices.length <= 100`
- `indices[i].length == 2`
- `0 <= indices[i][0] < m`
- `0 <= indices[i][1] < n`

## Follow-up

Filling in the whole board takes `O(m * n)` memory. Can you find the count
with `O(m + n + indices.length)` time while keeping only one number for
each row and one for each column?
