id: 1640
slug: check-array-formation-through-concatenation
title: Check Array Formation Through Concatenation
difficulty: easy
tags: array, hash-table
signature: bool canFormArray(int[] arr, int[][] pieces)
---
Each `pieces[i]` is a short run of numbers that must stay whole: it can't
be split, reversed or rearranged inside. You may, however, choose the
order of the pieces freely and then write them out one after another with
no gaps, which yields a single sequence.

Return `true` if some order of the pieces gives exactly `arr`, using every
piece once, and `false` if no order does.

No number occurs twice in `arr`, and no number occurs twice among all the
pieces taken together. The pieces hold as many numbers in all as `arr`
does, but not always the same ones: a piece may hold a number that `arr`
lacks, and then the answer is `false`.

@examples

## Constraints

- `1 <= pieces.length <= arr.length <= 100`
- `1 <= pieces[i].length <= arr.length`
- The lengths of all the pieces add up to `arr.length`.
- `1 <= arr[i], pieces[i][j] <= 100`
- The numbers of `arr` are distinct.
- The numbers of all the pieces, taken together, are distinct.
