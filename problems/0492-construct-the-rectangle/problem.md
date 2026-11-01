id: 492
slug: construct-the-rectangle
title: Construct the Rectangle
difficulty: easy
tags: math
signature: int[] constructRectangle(int area)
---
You want a rectangle whose sides `L` and `W` are positive whole numbers
and whose area `L * W` is exactly `area`. Call the longer side `L`, so
that `L >= W` (for a square, the two are equal).

Several rectangles may fit. Choose the one whose sides differ the least,
the one closest to a square, and return `[L, W]`, the longer side first.

There is always at least one, the strip of `area` by `1`, and never a tie:
of two rectangles that fit, the one with the larger `W` has the smaller
difference `L - W`.

@examples

## Constraints

- `1 <= area <= 10^7`
