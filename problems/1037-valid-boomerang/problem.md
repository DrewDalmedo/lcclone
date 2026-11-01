id: 1037
slug: valid-boomerang
title: Valid Boomerang
difficulty: easy
tags: geometry, array, math
signature: bool isBoomerang(int[][] points)
---
`points` has exactly three rows, and row `i` holds the integer coordinates
`[x, y]` of a point. Decide whether these three points form a
**boomerang**, which requires both of the following:

- the points are pairwise different: no two rows have both the same `x`
  and the same `y`;
- the points are not collinear: there is no straight line that touches
  all three at once.

Equivalently, the three points must be the vertices of a triangle whose
area is greater than zero. Return `true` for a boomerang and `false` for
anything else. Rearranging the rows never changes the answer.

@examples

## Constraints

- `points.length == 3`
- `points[i].length == 2`
- `0 <= points[i][j] <= 100`
