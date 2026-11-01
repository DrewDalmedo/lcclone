id: 657
slug: robot-return-to-origin
title: Robot Return to Origin
difficulty: easy
tags: string, simulation
signature: bool judgeCircle(string moves)
---
A robot walks on the points of an endless square grid. It starts at
`(0, 0)` and carries out the instructions in `moves` one at a time, from
the first character to the last. Each instruction takes it exactly one unit
in a direction that is fixed on the grid:

- `'U'` takes it from `(x, y)` up to `(x, y + 1)`;
- `'D'` takes it down to `(x, y - 1)`;
- `'R'` takes it right to `(x + 1, y)`;
- `'L'` takes it left to `(x - 1, y)`.

The robot never turns, so a letter means the same step wherever it comes in
`moves`. Return `true` if the robot is back at `(0, 0)` once the last
instruction is done, and `false` if it ends anywhere else. Only that final
point counts: visiting `(0, 0)` on the way does not make the answer `true`.

@examples

## Constraints

- `1 <= moves.length <= 2 * 10^4`
- Each `moves[i]` is `'U'`, `'D'`, `'L'` or `'R'`.
