id: 598
slug: range-addition-ii
title: Range Addition II
difficulty: easy
tags: array, math
signature: int maxCount(int m, int n, int[][] ops)
---
A grid has `m` rows, numbered `0` to `m - 1` from the top, and `n` columns,
numbered `0` to `n - 1` from the left. Every cell of it starts at `0`.

Each pair `[a, b]` in `ops` adds `1` to every cell whose row number is
less than `a` and whose column number is less than `b`. Those cells form a
rectangle, `a` rows high and `b` columns wide, in the top-left corner of the
grid; it never reaches past the grid's edges.

Apply all the pairs. Then find the largest number in the grid, and return
how many cells hold it. The answer is a count of cells, not the number
itself. If `ops` is empty, the grid stays all zeros, and all `m * n` of its
cells share the largest value.

@examples

## Constraints

- `1 <= m, n <= 4 * 10^4`
- `0 <= ops.length <= 10^4`
- `ops[i].length == 2`
- `1 <= ops[i][0] <= m`
- `1 <= ops[i][1] <= n`
