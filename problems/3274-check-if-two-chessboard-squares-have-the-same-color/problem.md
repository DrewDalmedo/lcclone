id: 3274
slug: check-if-two-chessboard-squares-have-the-same-color
title: Check if Two Chessboard Squares Have the Same Color
difficulty: easy
tags: math, string
signature: bool checkTwoChessboards(string coordinate1, string coordinate2)
---
A chessboard is a grid of 8 by 8 squares. Each square is named by two
characters: a letter from `a` to `h` for its column, counted from left to
right, then a digit from `1` to `8` for its row, counted from bottom to
top. So `a1` is the bottom-left square and `h8` the top-right one.

The squares are painted in two colours in a checkerboard pattern: two
squares that touch along an edge never have the same colour, while two
that touch only at a corner always do. To find the colour of any square,
number the letters `a` to `h` from 1 to 8 and add the row's digit. Every
square whose total is even has the colour of `a1`, and every square whose
total is odd has the other colour.

`coordinate1` and `coordinate2` each name a square, possibly the same one.
Return `true` if the two squares have the same colour, and `false` if
their colours differ.

@examples

## Constraints

- `coordinate1.length == coordinate2.length == 2`
- The first character of each is a lowercase letter from `'a'` to `'h'`,
  and the second is a digit from `'1'` to `'8'`.
