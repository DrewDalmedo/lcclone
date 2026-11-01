id: 1200
slug: minimum-absolute-difference
title: Minimum Absolute Difference
difficulty: easy
tags: array, sorting
signature: int[][] minimumAbsDifference(int[] arr)
---
Mark every number of `arr` as a point on a number line. No two of the
numbers are equal, so no two points coincide. Let `d` be the distance
between the two points that lie closest together, that is the smallest
value of `|x - y|` over two different numbers `x` and `y` of `arr`.

Return all the pairs of numbers of `arr` that are exactly `d` apart. Write
each pair as `[a, b]` with the smaller number first, so that `b - a == d`.
List the pairs as they appear along the line from left to right: in
increasing order of `a`. The numbers of `arr` come in no particular order,
and there is always at least one such pair.

@examples

## Constraints

- `2 <= arr.length <= 10^5`
- `-10^6 <= arr[i] <= 10^6`
- The numbers of `arr` are pairwise distinct.
