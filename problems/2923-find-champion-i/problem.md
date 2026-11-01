id: 2923
slug: find-champion-i
title: Find Champion I
difficulty: easy
tags: array, matrix
signature: int findChampion(int[][] grid)
---
A chess club has `n` players, labelled `0` to `n - 1`, and the club has
settled, for every two of them, which one is rated above the other.
`grid` is the `n x n` table of those verdicts: for two different players
`i` and `j`, `grid[i][j]` is `1` when player `i` is rated above player
`j`, and `0` when player `j` is rated above player `i`. So of
`grid[i][j]` and `grid[j][i]`, exactly one is `1`. A cell on the
diagonal, `grid[i][i]`, would compare a player with themself; it means
nothing and is always `0`.

The verdicts never contradict each other: if `a` is rated above `b` and
`b` above `c`, then `a` is rated above `c`. Together they line all the
players up in a single order from top to bottom, so exactly one player
has nobody rated above them. That player is the **champion**.

Return the champion's label.

@examples

## Constraints

- `n == grid.length == grid[i].length`
- `2 <= n <= 100`
- `grid[i][j]` is `0` or `1`.
- `grid[i][i] == 0`
- For `i != j`, `grid[i][j] != grid[j][i]`.
- If `grid[a][b] == 1` and `grid[b][c] == 1`, then `grid[a][c] == 1`.
