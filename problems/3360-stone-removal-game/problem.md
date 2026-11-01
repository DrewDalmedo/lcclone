id: 3360
slug: stone-removal-game
title: Stone Removal Game
difficulty: easy
tags: math, simulation
signature: bool canAliceWin(int n)
---
A heap of `n` stones lies between Alice and Bob, who move one after the
other, Alice opening. No one decides how big a move is: every move has a
**quota** fixed in advance and must remove exactly that many stones.
Alice's opening quota is `10`, and each later quota is one below the quota
of the move before it, which the other player made. The moves therefore
remove `10`, `9`, `8`, `7`, ... stones, alternating between Alice and Bob.

A move is possible only while the heap holds at least its quota; removing
the very last stones is allowed. A player who is to move but finds fewer
stones in the heap than the quota has lost the game, even when some stones
remain, and the opponent is the winner.

Return `true` if Alice wins, and `false` if Bob wins.

@examples

## Constraints

- `1 <= n <= 50`
