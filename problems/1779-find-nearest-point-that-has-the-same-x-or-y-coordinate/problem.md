id: 1779
slug: find-nearest-point-that-has-the-same-x-or-y-coordinate
title: Find Nearest Point That Has the Same X or Y Coordinate
difficulty: easy
tags: array
signature: int nearestValidPoint(int x, int y, int[][] points)
---
You stand at `(x, y)` on a grid of whole-number coordinates. Each entry
`points[i] = [a, b]` marks a point at `(a, b)`.

Call a point **in line** with you when it lies on the vertical line through
you (`a == x`), on the horizontal line through you (`b == y`), or on both,
which only the spot `(x, y)` itself does. Points that are not in line are
of no interest, no matter how near they are.

The distance to a point is `|a - x| + |b - y|`. For a point in line with
you one of the two terms is zero, so it is simply how far along the line
the point lies.

Return the index `i` of the in-line point at the smallest distance. If
several in-line points share that smallest distance, return the smallest
of their indices. If no point is in line with you, return `-1`. Indices
start at 0.

@examples

## Constraints

- `1 <= points.length <= 10^4`
- `points[i].length == 2`
- `1 <= x, y, points[i][0], points[i][1] <= 10^4`
