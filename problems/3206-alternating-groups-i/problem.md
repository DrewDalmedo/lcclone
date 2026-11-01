id: 3206
slug: alternating-groups-i
title: Alternating Groups I
difficulty: easy
tags: array, sliding-window
signature: int numberOfAlternatingGroups(int[] colors)
---
`n` beads are strung on a closed loop of string. `colors[i]` is the color of
bead `i`, either `0` or `1`, and the beads are listed in order around the
loop. The loop has no ends: after bead `n - 1` comes bead `0` again, so every
bead has exactly two neighbours, the bead before it and the bead after it.

Take any bead together with its two neighbours. These three beads form an
**alternating group** when the bead in the centre is the odd one out: its
color matches neither neighbour's. The loop holds `n` such triples, one
centred on each bead, and a triple may run across the point where bead
`n - 1` meets bead `0`.

Return how many of the `n` triples are alternating groups. Each triple is
judged on its own by its centre bead, so when `n = 3` the three triples are
counted separately even though they are made of the same three beads.

@examples

## Constraints

- `3 <= colors.length <= 100`
- Each `colors[i]` is `0` or `1`.
