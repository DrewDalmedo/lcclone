id: 3000
slug: maximum-area-of-longest-diagonal-rectangle
title: Maximum Area of Longest Diagonal Rectangle
difficulty: easy
tags: array
signature: int areaOfMaxDiagonal(int[][] dimensions)
---
A shop's price list describes its flat screens: screen `i` is a rectangle
with sides `dimensions[i][0]` and `dimensions[i][1]`. Screens are sold by
the length of their **diagonal**, the straight line from one corner to the
opposite one, which for sides `a` and `b` measures `sqrt(a^2 + b^2)`.

Pick out the screens whose diagonal is the longest in the list. Of those,
take the one with the largest area `a * b`, and return that area. A screen
with a shorter diagonal is never the answer, however large its area.

Diagonals are compared exactly. For instance `[1,8]` and `[4,7]` have the
same diagonal, since `1 + 64` and `16 + 49` are both `65`, though their
areas are 8 and 28.

@examples

## Constraints

- `1 <= dimensions.length <= 100`
- `dimensions[i].length == 2`
- `1 <= dimensions[i][0], dimensions[i][1] <= 100`
