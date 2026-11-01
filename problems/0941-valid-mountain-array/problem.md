id: 941
slug: valid-mountain-array
title: Valid Mountain Array
difficulty: easy
tags: array
signature: bool validMountainArray(int[] arr)
---
Walk through `arr` from left to right and look at each pair of
neighbours. The step from `arr[k]` to `arr[k + 1]` goes **up** when
`arr[k + 1]` is larger, **down** when it is smaller, and is **flat** when
the two are equal.

`arr` is a **mountain** when its steps are one or more up steps followed
by one or more down steps, and nothing else: no flat step anywhere, and
no up step after the first down step. So a mountain has at least three
elements, and its largest element is neither the first nor the last.

Return `true` if `arr` is a mountain, and `false` otherwise.

@examples

## Constraints

- `1 <= arr.length <= 10^4`
- `0 <= arr[i] <= 10^4`
