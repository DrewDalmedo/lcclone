id: 836
slug: rectangle-overlap
title: Rectangle Overlap
difficulty: easy
tags: geometry, math
signature: bool isRectangleOverlap(int[] rec1, int[] rec2)
---
On a map whose `x` axis points east and whose `y` axis points north, a
rectangular plot with its sides running east-west and north-south is
written `[x1, y1, x2, y2]`: `(x1, y1)` is its south-west corner and
`(x2, y2)` its north-east corner, so `x1 < x2` and `y1 < y2`.

Return `true` if the plots `rec1` and `rec2` overlap, meaning that some
region of positive area lies inside both of them, and `false` otherwise.
A shared boundary is not enough: if all the land two plots have in common
is a single corner post, or a stretch of fence along a side (the whole
side or part of it), that land has no area, and the answer is `false`. A
plot that lies within the other overlaps it, even when the two have a
side in common.

@examples

## Constraints

- `rec1.length == 4`
- `rec2.length == 4`
- `-10^9 <= rec1[i], rec2[i] <= 10^9`
- In each plot, `x1 < x2` and `y1 < y2`.
