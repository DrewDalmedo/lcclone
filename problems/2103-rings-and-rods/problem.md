id: 2103
slug: rings-and-rods
title: Rings and Rods
difficulty: easy
tags: hash-table, string
signature: int countPoints(string rings)
---
Ten upright rods stand in a row, labelled with the digits `0` to `9`.
Rings in three colours, red, green and blue, have been dropped over the
rods, and `rings` is the log of the drops, two characters per ring: first
the ring's colour, `R`, `G` or `B`, then the label of the rod it landed
on. So `"G5R0"` says that a green ring went onto rod 5 and a red one onto
rod 0.

A rod is **complete** when it holds at least one red, at least one green
and at least one blue ring. More rings of a colour the rod already has
add nothing, and the order of the drops doesn't matter.

Return how many of the ten rods are complete.

@examples

## Constraints

- `rings.length == 2 * n`, where `n` is the number of rings and
  `1 <= n <= 100`
- `rings[i]` is `'R'`, `'G'` or `'B'` when `i` is even (counting from 0),
  and a digit `'0'` to `'9'` when `i` is odd.
