id: 1725
slug: number-of-rectangles-that-can-form-the-largest-square
title: Number Of Rectangles That Can Form The Largest Square
difficulty: easy
tags: array
signature: int countGoodRectangles(int[][] rectangles)
---
Each pair `[l, w]` in `rectangles` describes a sheet of plywood, giving
its two side lengths in either order. You want to saw one square tile out
of a single sheet, with the tile's edges parallel to the sheet's. A tile
of side `k` fits on a sheet when `k` is no more than `l` and no more than
`w`, so the largest tile a sheet can yield is as long as its shorter side.
Tiles can't be glued together from pieces of several sheets.

Over all the sheets, find the largest tile side that any single sheet can
yield. Return the number of sheets that can yield a tile of exactly that
side. Each position in `rectangles` is its own sheet, even when two pairs
are equal.

@examples

## Constraints

- `1 <= rectangles.length <= 1000`
- `rectangles[i].length == 2`
- `1 <= rectangles[i][0], rectangles[i][1] <= 10^9`
- `rectangles[i][0] != rectangles[i][1]`
