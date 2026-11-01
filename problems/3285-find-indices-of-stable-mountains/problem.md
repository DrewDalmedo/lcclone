id: 3285
slug: find-indices-of-stable-mountains
title: Find Indices of Stable Mountains
difficulty: easy
tags: array
signature: int[] stableMountains(int[] height, int threshold)
compare: unordered 0
---
`height` lists the heights of `n` mountains, left to right: mountain `i` is
`height[i]` tall. Call mountain `i` **stable** when its left-hand
neighbour, mountain `i - 1`, is **strictly taller** than `threshold`, that
is, when `height[i - 1] > threshold`.

Only that neighbour matters: mountain `i`'s own height plays no part in
whether it is stable. Mountain `0` has nothing to its left, so it is never
stable.

Return the indices (counting from `0`) of every stable mountain, *in any
order*. If none is stable, return an empty array.

@examples

## Constraints

- `2 <= n == height.length <= 100`
- `1 <= height[i] <= 100`
- `1 <= threshold <= 100`
