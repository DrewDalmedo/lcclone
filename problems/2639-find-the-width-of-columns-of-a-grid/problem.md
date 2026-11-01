id: 2639
slug: find-the-width-of-columns-of-a-grid
title: Find the Width of Columns of a Grid
difficulty: easy
tags: array, matrix
signature: int[] findColumnWidth(int[][] grid)
---
`grid` is a table of integers with `m` rows and `n` columns. Picture it
printed as text, every number in its everyday base-10 form: a `-` in front
of a negative number, no sign in front of zero or a positive one, and no
leading zeros (zero is the single character `0`). So `-305` takes four
characters, `88` takes two and `0` takes one.

For the printout to line up, each column has to be as wide as the longest
number in it. The **width** of a column is therefore the largest character
count among its `m` numbers, with the `-` counted as a character.

Return an array of `n` integers whose entry `c` is the width of column `c`,
with the columns numbered from `0` on the left.

@examples

## Constraints

- `m == grid.length`
- `n == grid[i].length`
- `1 <= m, n <= 100`
- `-10^9 <= grid[i][j] <= 10^9`
