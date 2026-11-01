id: 4056
slug: number-of-intersecting-interval-pairs-i
title: Number of Intersecting Interval Pairs I
difficulty: easy
signature: int countIntersectingIntervals(int[][] intervals)
---
A tower has floors numbered from `0` to `100`, and each of its lifts serves
one unbroken stretch of them: `intervals[i] = [start, end]` says that lift
`i` stops at every floor from `start` up to `end`, **both included**. A lift
with `start == end` stops at that one floor only.

Two lifts **meet** when at least one floor is served by both. One shared
floor is enough: a lift serving floors `3` to `7` meets one serving floors
`7` to `12`, at floor `7`. A lift serving floors `3` to `7` does not meet
one serving floors `8` to `12`, as no floor is served by both.

Return how many pairs of lifts meet. A pair is two different lifts, counted
once whichever of them is listed first; a lift is never paired with itself.
Two lifts with the same stretch of floors are still two lifts, and they
meet. The lifts are listed in no particular order.

@examples

## Constraints

- `2 <= intervals.length <= 100`
- `intervals[i].length == 2`
- `0 <= intervals[i][0] <= intervals[i][1] <= 100`
