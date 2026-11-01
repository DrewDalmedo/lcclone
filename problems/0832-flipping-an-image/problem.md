id: 832
slug: flipping-an-image
title: Flipping an Image
difficulty: easy
tags: bit-manipulation, array, two-pointers, matrix, simulation
signature: int[][] flipAndInvertImage(int[][] image)
---
`image` describes a square slide with `n` rows of `n` dots: `image[i][j]`
is `1` when the dot in row `i`, column `j` is dark, and `0` when it is
clear. Make two changes to the slide:

1. **Turn it over** from left to right, as if you looked at it from the
   back. Each row stays where it is, but its dots now run the other way:
   the dot in column `j` moves to column `n - 1 - j`.
2. **Make it a negative**: every dark dot becomes clear, and every clear dot
   dark.

Return the slide after both changes, as `n` rows of `n` dots. (Making the
negative first and turning it over second gives the same slide.)

@examples

## Constraints

- `n == image.length`
- `n == image[i].length`
- `1 <= n <= 20`
- `image[i][j]` is `0` or `1`.
