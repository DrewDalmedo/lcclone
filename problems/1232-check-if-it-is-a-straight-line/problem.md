id: 1232
slug: check-if-it-is-a-straight-line
title: Check If It Is a Straight Line
difficulty: easy
tags: geometry, array, math
signature: bool checkStraightLine(int[][] coordinates)
---
Every entry of `coordinates` is a pair `[x, y]`: the point of the plane at
horizontal position `x` and vertical position `y`. No point is listed
twice.

Return `true` if all of the points lie on one straight line, and `false`
otherwise. That line may run in any direction, vertical and horizontal
included, and need not pass through `(0, 0)`. The points may come in any
order, not necessarily the order in which they lie along the line. With
just two points the answer is always `true`.

@examples

## Constraints

- `2 <= coordinates.length <= 1000`
- `coordinates[i].length == 2`
- `-10^4 <= coordinates[i][0], coordinates[i][1] <= 10^4`
- No point appears twice in `coordinates`.
