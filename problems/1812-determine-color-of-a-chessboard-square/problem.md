id: 1812
slug: determine-color-of-a-chessboard-square
title: Determine Color of a Chessboard Square
difficulty: easy
tags: math, string
signature: bool squareIsWhite(string coordinates)
---
Sit at a chessboard on White's side. The 8 columns, called **files**, are
lettered `a` to `h` from your left to your right, and the 8 rows, called
**ranks**, are numbered `1` to `8` going away from you. A square is named
by its file letter followed by its rank digit, so `a1` is the corner at
your left hand and `h8` the far corner on your right.

The squares are painted in two colours, light and dark, in the usual way:
the corner `h1`, at your right hand, is light, and any two squares that
share a side have different colours. So moving one square along a rank or
along a file always changes the colour.

`coordinates` names one square. Return `true` if that square is light (a
"white" square) and `false` if it is dark (a "black" square). The colours
are those of the board itself; no pieces are involved.

@examples

## Constraints

- `coordinates.length == 2`
- `'a' <= coordinates[0] <= 'h'`
- `'1' <= coordinates[1] <= '8'`
