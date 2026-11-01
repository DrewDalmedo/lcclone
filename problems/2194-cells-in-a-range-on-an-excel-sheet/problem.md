id: 2194
slug: cells-in-a-range-on-an-excel-sheet
title: Cells in a Range on an Excel Sheet
difficulty: easy
tags: string
signature: string[] cellsInRange(string s)
---
A small spreadsheet has 26 columns, lettered `A` to `Z` from left to right,
and 9 rows, numbered `1` to `9` from top to bottom. A cell is named by its
column letter followed by its row digit: `"C7"` is the cell in column `C`
and row `7`.

A rectangular block of cells is written as the names of its top-left and
bottom-right cells with a colon between them. `s` is such a block: `s[0]`
and `s[1]` name its top-left cell, and `s[3]` and `s[4]` its bottom-right
cell. The block holds every cell whose column is from `s[0]` to `s[3]` and
whose row is from `s[1]` to `s[4]`, both ends included.

Return the names of all cells of the block, one column at a time: first the
cells of the leftmost column, from the top row down, then the cells of the
next column to the right, from the top row down, and so on up to the
rightmost column.

@examples

## Constraints

- `s.length == 5`
- `'A' <= s[0] <= s[3] <= 'Z'`
- `'1' <= s[1] <= s[4] <= '9'`
- `s[2] == ':'`
