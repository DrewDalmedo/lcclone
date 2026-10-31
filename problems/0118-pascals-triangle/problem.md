id: 118
slug: pascals-triangle
title: Pascal's Triangle
difficulty: easy
tags: array, dynamic-programming
signature: int[][] generate(int numRows)
---
**Pascal's triangle** is built one row at a time, from the top. Rows are
numbered from 0, and so are the positions within a row, from the left. Row
0 holds a single 1. Row `i` holds `i + 1` numbers: it begins and ends with
a 1, and at each position `j` strictly between its ends (`0 < j < i`) it
holds number `j - 1` of row `i - 1` plus number `j` of row `i - 1`, the
pair that sits up-left and up-right of it.

Return rows 0 to `numRows - 1`, top row first, each as a list of its
numbers from left to right. The rows have different lengths: the first has
one number and the last has `numRows`.

@examples

## Constraints

- `1 <= numRows <= 30`
