id: 2500
slug: delete-greatest-value-in-each-row
title: Delete Greatest Value in Each Row
difficulty: easy
tags: array, matrix, sorting, simulation, heap-priority-queue
signature: int deleteGreatestValue(int[][] grid)
---
`grid` has `m` rows, and each row holds `n` positive integers. Empty it
in rounds, keeping a score that starts at `0`:

1. Every row gives up the largest number it still holds. If that number
   occurs in the row more than once, the row gives up only one copy of it.
2. Of the `m` numbers given up in this round, one per row, the largest is
   added to the score. The others are simply discarded.

Each round takes exactly one number out of every row, so after `n` rounds
the grid is empty. Return the score at that point.

@examples

## Constraints

- `m == grid.length`
- `n == grid[i].length`
- `1 <= m, n <= 50`
- `1 <= grid[i][j] <= 100`
