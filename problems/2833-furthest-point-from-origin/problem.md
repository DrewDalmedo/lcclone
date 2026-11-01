id: 2833
slug: furthest-point-from-origin
title: Furthest Point From Origin
difficulty: easy
tags: string, counting
signature: int furthestDistanceFromOrigin(string moves)
---
A token starts at `0` on the number line. The string `moves` is its
script: the token reads it left to right, takes one step per character,
and every step shifts it by exactly `1`:

- `'L'` moves it left, to the next smaller number;
- `'R'` moves it right, to the next larger number;
- `'_'` is a blank in the script. Before the token sets off, you fill in
  each blank with `'L'` or `'R'`, as you like, choosing for every blank on
  its own. So a blank still moves the token; it never leaves it in place.

When the token has taken all its steps, measure how far it ended up from
`0`: finishing at `-4` and finishing at `4` are both a distance of `4`.
Return the largest such distance that some way of filling in the blanks
gives.

The answer depends on the finishing point alone: wandering farther from `0`
partway through the script, and then coming back, earns nothing.

@examples

## Constraints

- `1 <= moves.length <= 50`
- `moves` consists only of the characters `'L'`, `'R'` and `'_'`.
