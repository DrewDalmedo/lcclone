id: 2614
slug: prime-in-diagonal
title: Prime In Diagonal
difficulty: easy
tags: array, math, matrix, number-theory
signature: int diagonalPrime(int[][] nums)
---
`nums` is a square grid with `n` rows and `n` columns, both numbered from
`0`. The cell in row `r` and column `c` is a **diagonal cell** when
`r == c` or when `r + c == n - 1`. Only diagonal cells matter here: a number
anywhere else is never part of the answer, however large it is.

Look at the numbers held in diagonal cells and return the largest of them
that is prime. If none of them is prime, return `0`.

Call a number **prime** when it is at least `2` and cannot be written as a
product `a * b` of two whole numbers that are both at least `2`. So `1` and
`4 = 2 * 2` are not prime, while `2` and `3` are.

@examples

## Constraints

- `n == nums.length == nums[i].length`
- `1 <= n <= 300`
- `1 <= nums[i][j] <= 4 * 10^6`
