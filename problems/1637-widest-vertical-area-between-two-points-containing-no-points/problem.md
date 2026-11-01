id: 1637
slug: widest-vertical-area-between-two-points-containing-no-points
title: Widest Vertical Area Between Two Points Containing No Points
difficulty: easy
tags: array, sorting
signature: int maxWidthOfVerticalArea(int[][] points)
---
Each entry `points[i] = [x, y]` is a point in the plane. Two points may
coincide or share an x-coordinate.

Pick two of the points and draw a vertical line through each: the lines
`X = a` and `X = b`, where `a <= b` are the x-coordinates of the two
points. The region strictly between the two lines is a **strip** of width
`b - a`. It runs up and down without end, so only x-coordinates matter: a
point lies **inside** the strip when `a < x < b`. A point exactly on one of
the two lines is not inside.

Return the largest width of a strip that has no point inside. Both lines of
a strip must go through given points, so the open space to the left of the
leftmost point, or to the right of the rightmost one, doesn't count.
If every point has the same x-coordinate, the answer is `0`.

@examples

## Constraints

- `n == points.length`
- `2 <= n <= 10^5`
- `points[i].length == 2`
- `0 <= points[i][0], points[i][1] <= 10^9`
