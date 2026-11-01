id: 2951
slug: find-the-peaks
title: Find the Peaks
difficulty: easy
tags: array, enumeration
signature: int[] findPeaks(int[] mountain)
compare: unordered 0
---
`mountain` lists the heights of a mountain range from left to right. Index
`i` is a **peak** when it has a neighbour on each side and is **strictly
taller** than both: `mountain[i] > mountain[i - 1]` and
`mountain[i] > mountain[i + 1]`.

The first and the last index have only one neighbour, so they are never
peaks, however tall they are. An index as tall as one of its neighbours is
not a peak either.

Return the indices (counting from `0`) of every peak, *in any order*. If
there are no peaks, return an empty array.

@examples

## Constraints

- `3 <= mountain.length <= 100`
- `1 <= mountain[i] <= 100`
