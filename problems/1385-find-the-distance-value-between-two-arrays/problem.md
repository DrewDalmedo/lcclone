id: 1385
slug: find-the-distance-value-between-two-arrays
title: Find the Distance Value Between Two Arrays
difficulty: easy
tags: array, two-pointers, binary-search, sorting
signature: int findTheDistanceValue(int[] arr1, int[] arr2, int d)
---
An entry `arr1[i]` keeps **clear** of `arr2` when every entry of `arr2`
is more than `d` away from it, that is, when `|arr1[i] - arr2[j]| > d` for
every `j`. Put the other way round: no entry of `arr2` lies between
`arr1[i] - d` and `arr1[i] + d`, both ends included.

Return the **distance value** of the two arrays: how many entries of
`arr1` keep clear of `arr2`.

A gap of exactly `d` is too close: such an entry does not count. Entries
are counted by position, so a value that occurs several times in `arr1`
counts once for each occurrence that keeps clear. Neither array needs to
be sorted.

@examples

## Constraints

- `1 <= arr1.length, arr2.length <= 500`
- `-1000 <= arr1[i], arr2[j] <= 1000`
- `0 <= d <= 100`
