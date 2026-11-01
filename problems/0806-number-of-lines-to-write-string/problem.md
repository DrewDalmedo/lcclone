id: 806
slug: number-of-lines-to-write-string
title: Number of Lines To Write String
difficulty: easy
tags: array, string
signature: int[] numberOfLines(int[] widths, string s)
---
A sign maker puts up the letters of `s` on a board, in rows, and each row has
room for 100 units of width. Letters can differ in width: the letter
`'a' + k` is `widths[k]` units wide, so `widths[0]` is the width of `a`,
`widths[1]` that of `b`, and so on up to `widths[25]` for `z`.

The letters go up one at a time, in the order of `s`, starting on the first
row:

- if the letter fits on the current row, that is, the widths of the letters
  already on it plus its own add up to at most 100, it goes on that row;
- otherwise that row is finished, and the letter goes first on a new row.

A row may be filled to exactly 100. Letters are never split, skipped or
reordered, and nothing goes back onto a finished row later, even a letter
narrow enough to fit in the room it has left.

Return an array of two numbers: how many rows the letters take, and the
width of the last row, the sum of the widths of the letters on it.

@examples

## Constraints

- `widths.length == 26`
- `2 <= widths[k] <= 10`
- `1 <= s.length <= 1000`
- `s` consists of lowercase English letters.
