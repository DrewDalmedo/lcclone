id: 2138
slug: divide-a-string-into-groups-of-size-k
title: Divide a String Into Groups of Size k
difficulty: easy
tags: string, simulation
signature: string[] divideString(string s, int k, char fill)
---
Copy the letters of `s` into a grid that is `k` cells wide, one letter per
cell, keeping their order: fill the top row from left to right, then the
row below it, and so on, moving to a new row only once the current one is
full. The grid has just as many rows as the letters need, so no row is
left without a letter of `s`.

When the letters run out partway along the bottom row, write the letter
`fill` into each of its remaining cells. When the length of `s` is a
multiple of `k`, the bottom row is already full and `fill` is not used.

Return the rows of the grid from top to bottom, each as a string of
exactly `k` letters. `fill` may also be a letter that occurs in `s`; the
padding is only ever added, and the letters of `s` stay as they are.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters.
- `1 <= k <= 100`
- `fill` is a lowercase English letter.
