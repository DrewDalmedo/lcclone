id: 1025
slug: divisor-game
title: Divisor Game
difficulty: easy
tags: brainteaser, math, dynamic-programming, game-theory
signature: bool divisorGame(int n)
---
A marker starts on square `n` of a track whose squares are numbered `1`,
`2`, `3`, and so on. Two players take turns moving it; the one who makes
the very first move is called the first player.

On a turn, if the marker is on square `m`, the player picks a number `d`
that divides `m` exactly and is smaller than `m` (so `1 <= d < m`), and
moves the marker back `d` squares, to square `m - d`. Passing is not
allowed. The player who has no possible move, which happens only when the
marker is on square `1`, loses the game.

Neither player ever makes a mistake: on each turn, if some move lets the
player to move force a win from there, that player makes such a move.
Return `true` if the first player wins, and `false` if the second player
does.

@examples

## Constraints

- `1 <= n <= 1000`
