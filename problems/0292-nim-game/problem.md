id: 292
slug: nim-game
title: Nim Game
difficulty: easy
tags: brainteaser, math, game-theory
signature: bool canWinNim(int n)
---
A single pile holds `n` pebbles. Two players take turns removing pebbles
from it, and you make the first move. A move removes one, two or three
pebbles, never more than the pile still holds, and no player may skip a
turn. Whoever removes the last pebble wins; the player left facing an
empty pile has lost.

Both players play perfectly: whenever some move leads to a win, the player
to move picks one. Return `true` if you, moving first, can win whatever
your opponent does, and `false` if your opponent can always beat you.

@examples

## Constraints

- `1 <= n <= 2^31 - 1`
