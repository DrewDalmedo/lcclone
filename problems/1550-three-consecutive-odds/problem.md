id: 1550
slug: three-consecutive-odds
title: Three Consecutive Odds
difficulty: easy
tags: array
signature: bool threeConsecutiveOdds(int[] arr)
---
Slide a window three elements wide along `arr`: it covers `arr[i]`,
`arr[i + 1]` and `arr[i + 2]`, for every `i` at which all three exist.
Return `true` if at some place all three numbers in the window are odd, and
`false` otherwise. An array of fewer than three elements has no window at
all, so its answer is `false`.

Only the positions have to follow one another. The three odd numbers
themselves can have any values: `7, 7, 7` counts, and so does `9, 1, 25`.
More than three odd numbers in a row count as well, since any three
neighbours among them fill a window.

@examples

## Constraints

- `1 <= arr.length <= 1000`
- `1 <= arr[i] <= 1000`
