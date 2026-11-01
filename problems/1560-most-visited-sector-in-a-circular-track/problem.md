id: 1560
slug: most-visited-sector-in-a-circular-track
title: Most Visited Sector in  a Circular Track
difficulty: easy
tags: array, simulation
signature: int[] mostVisited(int n, int[] rounds)
---
A night guard patrols a ring of `n` rooms, numbered `1` to `n`. Each room
opens into the next, and the guard always moves in the same direction:
from room `k` on to room `k + 1`, and from room `n` back to room `1`.

His patrol is planned as the list `rounds` of `m + 1` room numbers. He
starts in room `rounds[0]`. Then, for each `i` from `1` to `m` in turn, he
walks on from room `rounds[i - 1]` until he first reaches room `rounds[i]`,
where the next stretch begins. Two neighbouring entries of `rounds` are
never the same room, so each stretch takes at least one step and less than
a full turn of the ring.

Count the visits each room gets. Room `rounds[0]` gets one for being where
the guard starts; after that, every step into a room is one visit to that
room. A room where one stretch ends and the next begins is visited once
there, not twice.

Return the numbers of all the rooms with the most visits, from smallest to
largest.

@examples

## Constraints

- `2 <= n <= 100`
- `1 <= m <= 100`
- `rounds.length == m + 1`
- `1 <= rounds[i] <= n`
- `rounds[i] != rounds[i + 1]` for `0 <= i < m`
