id: 661
slug: image-smoother
title: Image Smoother
difficulty: easy
tags: array, matrix
signature: int[][] imageSmoother(int[][] img)
---
`img` is a grayscale picture `m` pixels high and `n` pixels wide:
`img[i][j]` is the brightness of the pixel in row `i` and column `j`, from
`0` (black) to `255` (white). Return a blurred copy of it, a new grid of the
same size, made as follows.

The **neighbourhood** of a pixel is the pixel itself together with every
pixel that touches it, along a side or only at a corner: at most 9 pixels,
filling the 3 by 3 square around it. Places of that square that fall
outside the picture are simply left out. So in a picture at least 2 pixels
high and wide, a corner pixel's neighbourhood has 4 pixels, and in a
picture 1 pixel high, no neighbourhood has more than 3.

In the blurred copy, each pixel gets the average brightness of its
neighbourhood in `img`: the sum of those brightnesses divided by how many
pixels the neighbourhood has, rounded down to a whole number. Every average
is taken over the original picture, never over pixels already blurred.

@examples

## Constraints

- `m == img.length`
- `n == img[i].length`
- `1 <= m, n <= 200`
- `0 <= img[i][j] <= 255`
