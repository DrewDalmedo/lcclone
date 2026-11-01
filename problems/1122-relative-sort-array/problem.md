id: 1122
slug: relative-sort-array
title: Relative Sort Array
difficulty: easy
tags: array, hash-table, counting-sort, sorting
signature: int[] relativeSortArray(int[] arr1, int[] arr2)
---
`arr2` is a list of distinct values that sets a custom order, and every
value in it also occurs somewhere in `arr1` (`arr1` may hold repeated
values). Rearrange the values of `arr1`, keeping each one as many times
as it occurs, into two parts, and return the result:

1. **Listed values.** First come the values that `arr2` lists, grouped
   and in the order `arr2` gives them: every copy of `arr2[0]`, then
   every copy of `arr2[1]`, and so on up to the last value of `arr2`.
2. **The rest.** Then come the values of `arr1` that `arr2` does not
   list, from smallest to largest, with repeated values side by side.

The positions of the values within `arr1` play no part. The listed values
follow `arr2`'s order even when it isn't increasing, and the rest always
come after them, even when they are smaller.

@examples

## Constraints

- `1 <= arr1.length, arr2.length <= 1000`
- `0 <= arr1[i], arr2[i] <= 1000`
- The values of `arr2` are distinct.
- Every value of `arr2` occurs in `arr1`.
