id: 3222
slug: find-the-winning-player-in-coin-game
title: Find the Winning Player in Coin Game
difficulty: easy
tags: math, game-theory, simulation
signature: string winningPlayer(int x, int y)
---
Alice and Bob share one jar of coins: `x` coins worth 75 each and `y`
coins worth 10 each. They take turns, and Alice goes first.

On a turn, the player whose turn it is must take out of the jar some coins
whose values add up to exactly 115. Coins that are taken out stay out for
the rest of the game. The first player who can't do this on their turn
loses, and the other player wins.

Both players always choose their moves as well as they can. Return the
winner's name: exactly `"Alice"` or `"Bob"`.

@examples

## Constraints

- `1 <= x <= 100`
- `1 <= y <= 100`
