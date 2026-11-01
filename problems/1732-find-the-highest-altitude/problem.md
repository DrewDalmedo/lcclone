id: 1732
slug: find-the-highest-altitude
title: Find the Highest Altitude
difficulty: easy
tags: array, prefix-sum
signature: int largestAltitude(int[] gain)
---
A trail is marked by `n + 1` posts, numbered `0` to `n` in the order you
pass them. Post `0`, where you start, stands at altitude `0`. For each `i`,
`gain[i]` is how much the altitude changes from post `i` to post `i + 1`:
a positive value means the trail climbs, a negative one that it descends,
and `0` that it stays level.

Look at every post, the starting one and the last one included, pick the
one that stands highest, and return its altitude. Since post `0` is among
them, the answer is never below `0`.

@examples

## Constraints

- `n == gain.length`
- `1 <= n <= 100`
- `-100 <= gain[i] <= 100`
