id: 2315
slug: count-asterisks
title: Count Asterisks
difficulty: easy
tags: string
signature: int countAsterisks(string s)
---
The string `s` is made of lowercase letters, stars `*` and vertical bars
`|`. Walk along it from left to right. You start **outdoors**, and each bar
you step over is a door: it takes you indoors if you were outdoors, and back
outdoors if you were indoors.

Counting the bars from the left, the stretch from bar 1 to bar 2 is
indoors, from bar 2 to bar 3 outdoors again, from bar 3 to bar 4 indoors,
and the stretches keep alternating like that. Whatever comes before the
first bar or after the last one is outdoors. Two bars side by side enclose
an empty stretch.

Return how many stars you pass while outdoors. Stars passed indoors don't
count, and neither do letters or bars. `s` always has an even number of
bars, possibly none, so the walk ends outdoors; with no bars at all, every
star counts.

@examples

## Constraints

- `1 <= s.length <= 1000`
- `s` consists of lowercase English letters, `'*'` and `'|'`.
- The number of `'|'` in `s` is even (it may be `0`).
