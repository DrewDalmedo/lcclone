id: 2660
slug: determine-the-winner-of-a-bowling-game
title: Determine the Winner of a Bowling Game
difficulty: easy
tags: array, simulation
signature: int isWinner(int[] player1, int[] player2)
---
Two players bowl the same number of rounds, `n`. Ten pins are set up for
every round, and `player1[i]` and `player2[i]` say how many of them each
player knocked down in round `i` (rounds are numbered from `0`). A round in
which a player knocks down all `10` pins is a **strike** for that player.

Each player is scored on their own rounds only, by this rule (not by the
usual rules of bowling):

- a round is worth **twice** its pins when the same player had a strike in
  the round just before it, or in the round two before it, or in both;
- any other round is worth just its pins.

Doubling happens at most once: a round that follows two strikes is still
worth twice its pins, not four times. Whether a round was a strike depends
only on its pins being `10`, not on what the round was worth. Round `0` has
no rounds before it, so it is never doubled, and round `1` is doubled only
after a strike in round `0`.

Add up what each player's rounds are worth to get that player's total.
Compare the two totals: return `1` when player 1's is higher, `2` when
player 2's is higher, or `0` for a tie.

@examples

## Constraints

- `n == player1.length == player2.length`
- `1 <= n <= 1000`
- `0 <= player1[i], player2[i] <= 10`
