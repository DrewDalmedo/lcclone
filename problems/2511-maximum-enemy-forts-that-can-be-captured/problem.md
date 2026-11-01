id: 2511
slug: maximum-enemy-forts-that-can-be-captured
title: Maximum Enemy Forts That Can Be Captured
difficulty: easy
tags: array, two-pointers
signature: int captureForts(int[] forts)
---
`forts` is a map of positions along a straight frontier, from left to
right. Each position holds one of three things:

- `1`: a fort of yours;
- `0`: an enemy fort;
- `-1`: nothing; the position is empty.

You may send out one army. It starts at a position holding `1`, travels
in a straight line, to the left or to the right, and stops at a position
holding `-1`. On the way it may cross only enemy forts: every position
strictly between its start and its stop must hold `0`, so it can't pass
another fort of yours or another empty position. Each enemy fort it
crosses is captured.

Return the largest number of enemy forts one army can capture. Return `0`
if no army can be sent out at all: when you have no fort, when no
position is empty, or when every route is blocked. An army that moves to
an empty position right next to its fort is allowed, and captures `0`.

@examples

## Constraints

- `1 <= forts.length <= 1000`
- Each `forts[i]` is `-1`, `0` or `1`.
