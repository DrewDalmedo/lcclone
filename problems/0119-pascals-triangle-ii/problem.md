id: 119
slug: pascals-triangle-ii
title: Pascal's Triangle II
difficulty: easy
tags: array, dynamic-programming
signature: int[] getRow(int rowIndex)
---
**Pascal's triangle** is built one row at a time, from the top. Rows are
numbered from 0, and so are the positions within a row, from the left. Row
0 holds a single 1. Row `i` holds `i + 1` numbers: it begins and ends with
a 1, and at each position `j` strictly between its ends (`0 < j < i`) it
holds number `j - 1` of row `i - 1` plus number `j` of row `i - 1`, the
pair that sits up-left and up-right of it.

Return row `rowIndex` alone, as a list of its `rowIndex + 1` numbers from
left to right.

Every number of the rows asked for fits in a 32-bit `int`, though the
largest are over `10^9`.

@examples

## Constraints

- `0 <= rowIndex <= 33`

## Follow-up

Building the whole triangle keeps every row above the one you want. Can you
get there with a row or two's worth of storage, `O(rowIndex)` numbers,
instead?
