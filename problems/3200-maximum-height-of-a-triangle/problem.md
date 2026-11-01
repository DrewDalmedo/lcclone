id: 3200
slug: maximum-height-of-a-triangle
title: Maximum Height of a Triangle
difficulty: easy
tags: array, enumeration
signature: int maxHeightOfTriangle(int red, int blue)
---
You have some red balls and some blue balls: `red` counts the red ones and
`blue` the blue ones. You want to stack them into a triangle as tall as
possible. A triangle of **height** `h` has `h` rows. Its top row is a single
ball, and each row under it holds one ball more than the row above, so the
`k`-th row from the top is made of exactly `k` balls. Every row must be
complete.

Two color rules apply:

- no row mixes colors: it is either all red or all blue;
- a row and the row right under it never share a color, so the colors take
  turns from the top down.

Either color may go in the top row, and balls that aren't needed are simply
left over. Return the greatest height you can build.

@examples

## Constraints

- `1 <= red <= 100`
- `1 <= blue <= 100`
