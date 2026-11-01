id: 1460
slug: make-two-arrays-equal-by-reversing-subarrays
title: Make Two Arrays Equal by Reversing Subarrays
difficulty: easy
tags: array, hash-table, sorting
signature: bool canBeEqual(int[] target, int[] arr)
---
`target` and `arr` are integer arrays of the same length. `target` stays as
it is, and `arr` can be changed only by **flips**. A flip picks a block of
one or more neighbouring positions of `arr` and turns that block back to
front: its first element trades places with its last, its second with its
second-to-last, and so on, while everything outside the block stays put.

You may make as many flips as you like, one after another, and blocks of
different flips may overlap. Making no flip at all is allowed too.

Return `true` if some series of flips leaves `arr` holding the same value
as `target` at every position, and `false` if no series does.

@examples

## Constraints

- `target.length == arr.length`
- `1 <= target.length <= 1000`
- `1 <= target[i] <= 1000`
- `1 <= arr[i] <= 1000`
