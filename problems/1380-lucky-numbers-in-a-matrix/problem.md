id: 1380
slug: lucky-numbers-in-a-matrix
title: Lucky Numbers in a Matrix
difficulty: easy
tags: array, matrix
signature: int[] luckyNumbers(int[][] matrix)
compare: unordered 0
---
`matrix` has `m` rows of `n` integers each, and no number appears in it
twice. A number of `matrix` is **lucky** when it is the smallest number in
its own row and, at the same time, the largest number in its own column.

Return the lucky numbers of `matrix` (the values, not their positions), *in
any order*. If no number is lucky, return an empty array.

@examples

## Constraints

- `m == matrix.length`
- `n == matrix[i].length`
- `1 <= m, n <= 50`
- `1 <= matrix[i][j] <= 10^5`
- All the numbers of `matrix` are distinct.

## Follow-up

Comparing every number with its whole row and column takes
`O(m * n * (m + n))` time. Can you do it in `O(m * n)`? And how many lucky
numbers can a matrix of distinct numbers have at most?
