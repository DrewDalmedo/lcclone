id: 1013
slug: partition-array-into-three-parts-with-equal-sum
title: Partition Array Into Three Parts With Equal Sum
difficulty: easy
tags: greedy, array
signature: bool canThreePartsEqualSum(int[] arr)
---
Make two cuts in the array `arr`, so that it falls into three pieces: a
front, a middle and a back. The elements keep their order, every element
ends up in exactly one piece, and each piece must hold **at least one**
element. So if the front ends at index `i` and the back starts at index
`j`, then `0 <= i`, `i + 2 <= j` and `j <= n - 1`, where `n` is the length
of `arr`.

Return `true` if the cuts can be placed so that the three pieces have the
same sum, and `false` if no placement does. The elements may be negative or
zero, so the common sum may be negative or zero as well.

@examples

## Constraints

- `3 <= arr.length <= 5 * 10^4`
- `-10^4 <= arr[i] <= 10^4`
