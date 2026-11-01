id: 999
slug: available-captures-for-rook
title: Available Captures for Rook
difficulty: easy
tags: array, matrix, simulation
signature: int numRookCaptures(char[][] board)
---
`board` is a chess position on an 8 x 8 board, given row by row. Each
square holds one character:

- `'R'`: the white rook. There is exactly one.
- `'B'`: a white bishop, on the same side as the rook.
- `'p'`: a black pawn, on the other side.
- `'.'`: an empty square.

Seen from the rook, its row and its column make four **rays**: up, down,
left and right. Walk along a ray from the square next to the rook towards
the edge of the board. The first piece on it, if there is one, is the only
piece the rook can reach in that direction; whatever lies further along is
shielded by it.

A pawn is **under attack** when it is the first piece on one of the four
rays. A bishop that comes first only blocks the way, since the rook never
takes a piece of its own side. Return the number of pawns under attack.
Each ray holds at most one of them, so the answer is between `0` and `4`.
Squares outside the rook's row and column, diagonals included, never
matter.

@examples

## Constraints

- `board.length == 8`
- `board[i].length == 8`
- `board[i][j]` is `'R'`, `'B'`, `'p'` or `'.'`.
- Exactly one square of `board` is `'R'`.
