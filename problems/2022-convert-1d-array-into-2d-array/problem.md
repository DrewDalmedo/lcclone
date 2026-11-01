id: 2022
slug: convert-1d-array-into-2d-array
title: Convert 1D Array Into 2D Array
difficulty: easy
tags: array, matrix, simulation
signature: int[][] construct2DArray(int[] original, int m, int n)
---
Cut the list `original` into consecutive pieces of `n` numbers each,
starting from the front, and stack the pieces in that order as the rows of
a matrix: the first `n` numbers are row `0`, the next `n` are row `1`, and
so on. The numbers keep their order and their values.

Return that matrix if it has exactly `m` rows, each holding all `n`
numbers. Every number of `original` has to be used exactly once and every
row has to be full, so this works only when `original` has exactly
`m * n` numbers. Otherwise (numbers would be left over, or there are too
few to fill `m` rows of `n`) return an empty matrix: `[]`, with no rows at
all.

@examples

## Constraints

- `1 <= original.length <= 5 * 10^4`
- `1 <= original[i] <= 10^5`
- `1 <= m, n <= 4 * 10^4`
