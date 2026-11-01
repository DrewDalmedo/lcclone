id: 2682
slug: find-the-losers-of-the-circular-game
title: Find the Losers of the Circular Game
difficulty: easy
tags: array, hash-table, simulation
signature: int[] circularGameLosers(int n, int k)
---
A round pond has `n` lily pads in a ring, labelled `1` to `n` going
clockwise, so one pad clockwise of pad `n` is pad `1` again. A frog starts
on pad `1`, which already counts as a pad it has landed on, and jumps
clockwise:

- its `t`-th jump (`t = 1, 2, 3, ...`) is `t * k` pads long, counted from
  the pad it jumps from: `k` pads, then `2 * k`, then `3 * k`, and so on;
- a jump longer than the ring simply goes round it as many times as
  needed;
- the frog stops as soon as it lands on a pad it has landed on before,
  pad `1` included. A jump whose length is a multiple of `n` brings it
  back to the pad it jumped from, so it stops there.

Call a pad a **loser** if the frog never lands on it. Return the labels of
all loser pads in increasing order, or an empty array if the frog lands on
every pad.

@examples

## Constraints

- `1 <= k <= n <= 50`
