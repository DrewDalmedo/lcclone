id: 3248
slug: snake-in-matrix
title: Snake in Matrix
difficulty: easy
tags: array, string, simulation
signature: int finalPositionOfSnake(int n, string[] commands)
---
A square board has `n` rows and `n` columns. Its squares carry the labels
`0` to `n * n - 1`, handed out in reading order: along the top row from left
to right, then along the next row down, and so on. Counting rows from the top
and columns from the left, both from `0`, the square in row `r` and column `c`
is labelled `r * n + c`.

A snake starts on square `0`, in the top-left corner. Only its head matters
here, so think of it as a single square that moves. `commands` lists its
moves in order, and each one shifts it by exactly one square:

- `"UP"` to the row above (`r - 1`);
- `"DOWN"` to the row below (`r + 1`);
- `"LEFT"` to the column on its left (`c - 1`);
- `"RIGHT"` to the column on its right (`c + 1`).

No move ever takes the snake off the board. Return the label of the square
it is on once every command has been carried out.

@examples

## Constraints

- `2 <= n <= 10`
- `1 <= commands.length <= 100`
- Each `commands[i]` is `"UP"`, `"DOWN"`, `"LEFT"` or `"RIGHT"`.
- The snake stays on the board after every move.
