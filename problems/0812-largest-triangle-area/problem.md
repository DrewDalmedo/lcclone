id: 812
slug: largest-triangle-area
title: Largest Triangle Area
difficulty: easy
tags: geometry, array, math, polygon
signature: double largestTriangleArea(int[][] points)
---
Each `points[i] = [x, y]` is a point of the plane with whole-number
coordinates, and no two points are the same. Every choice of three of the
points gives a triangle with those three corners. Return the largest area
among all these triangles.

Three points on one straight line make a flat triangle of area `0`, so if
all the points lie on one line, the answer is `0`.

An answer within `10^-5` of the exact area is accepted.

@examples

## Constraints

- `3 <= points.length <= 50`
- `points[i].length == 2`
- `-50 <= x, y <= 50`
- All the points are different.

## Follow-up

Trying every triple of points takes `O(n^3)` time. Which points can never
be a corner of the largest triangle?
