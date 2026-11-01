id: 1275
slug: find-winner-on-a-tic-tac-toe-game
title: Find Winner on a Tic Tac Toe Game
difficulty: easy
tags: array, hash-table, matrix, simulation
signature: string tictactoe(int[][] moves)
---
`moves` is the record of a game of tic-tac-toe, move by move, on a board
of 3 rows and 3 columns that starts out empty. Two players take part.
Player `A` moves first and the turns then alternate, so `moves[0]`,
`moves[2]`, `moves[4]`, ... are `A`'s and `moves[1]`, `moves[3]`, ... are
`B`'s. A move `[r, c]` claims the square in row `r` and column `c` (both
counted from `0`) for the player who makes it.

A player **wins** on owning all three squares of a line. There are eight
lines: the three rows, the three columns, and the two diagonals, one
through `[0,0]`, `[1,1]` and `[2,2]`, the other through `[0,2]`, `[1,1]`
and `[2,0]`. The game is over as soon as a player wins, or when all nine
squares are claimed. Every move in `moves` is legal: it claims a square
nobody owns yet, and no move comes after the game is over.

Return the state of the game after the last move of the record:

- `"A"` or `"B"` if that player has won;
- `"Draw"` if nobody has won and all nine squares are claimed;
- `"Pending"` if nobody has won and some square is still free, even when
  no player can complete a line any more.

A win on the ninth move is a win, not a draw.

@examples

## Constraints

- `1 <= moves.length <= 9`
- `moves[i].length == 2`
- `0 <= moves[i][0], moves[i][1] <= 2`
- No square appears in `moves` twice.
- `moves` follows the rules above.
