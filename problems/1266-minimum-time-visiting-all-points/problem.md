id: 1266
slug: minimum-time-visiting-all-points
title: Minimum Time Visiting All Points
difficulty: easy
tags: geometry, array, math
signature: int minTimeToVisitAllPoints(int[][] points)
---
Each entry of `points` is a pair `[x, y]`: a point of the plane whose
coordinates are integers. A token travels between such points. Each
second it makes one move: it changes its `x` by `-1`, `0` or `1` and, at
the same time, its `y` by `-1`, `0` or `1` (not both by `0`). That is how
a king moves on a chessboard, so a diagonal move takes one second, the
same as a move straight along `x` or along `y`.

The token starts on `points[0]`, where the clock reads `0`, and must
reach `points[1]`, then `points[2]`, and so on up to the last point,
strictly in that order. Landing on a point before its turn comes doesn't
count as reaching it. The trip ends at the last point; the token doesn't
return to the first one.

Return the smallest number of seconds the whole trip can take.

@examples

## Constraints

- `1 <= points.length <= 100`
- `points[i].length == 2`
- `-1000 <= points[i][0], points[i][1] <= 1000`
