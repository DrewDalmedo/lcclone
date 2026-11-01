id: 3996
slug: even-number-of-knight-moves
title: Even Number of Knight Moves
difficulty: easy
tags: array, math
signature: bool canReach(int[] start, int[] target)
---
The squares of an 8 by 8 board are given as pairs `[x, y]`, where `x` and
`y` are whole numbers from `0` to `7`. A knight stands on the square
`start`. From `[x, y]` it can jump to `[x + a, y + b]` whenever one of `a`
and `b` is `1` or `-1` and the other is `2` or `-2`, provided both
coordinates of the landing square are still between `0` and `7`. That
gives it at most eight jumps to choose from.

Decide whether some route made of an **even** number of jumps ends with
the knight on the square `target`: return `true` if there is one, and
`false` if there is none. A route may pass through any square as often as
it likes, `start` and `target` included. `start` and `target` may be the
same square, and making no jumps at all counts as an even number (zero).

@examples

## Constraints

- `start.length == target.length == 2`
- `0 <= start[i], target[i] <= 7`
