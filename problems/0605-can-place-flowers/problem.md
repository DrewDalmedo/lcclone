id: 605
slug: can-place-flowers
title: Can Place Flowers
difficulty: easy
tags: greedy, array
signature: bool canPlaceFlowers(int[] flowerbed, int n)
---
A garden bed is a single straight row of plots, described by `flowerbed`:
`flowerbed[i]` is `1` when plot `i` already holds a flower and `0` when it
is bare. Two plots are neighbours when their numbers differ by 1; the row
does not wrap around from its last plot back to its first. A flower needs
room, so no two flowers may ever be in neighbouring plots. The bed you are
given already keeps this rule.

You want to add `n` new flowers, each in a bare plot, without moving any of
the flowers already there. Return `true` if all `n` of them can go in with
no two flowers, old or new, in neighbouring plots, and `false` if there is
no way to fit them all.

@examples

## Constraints

- `1 <= flowerbed.length <= 2 * 10^4`
- Each `flowerbed[i]` is `0` or `1`.
- No two `1`s of `flowerbed` are next to each other.
- `0 <= n <= flowerbed.length`
