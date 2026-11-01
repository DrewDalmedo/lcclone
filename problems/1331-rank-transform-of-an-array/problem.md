id: 1331
slug: rank-transform-of-an-array
title: Rank Transform of an Array
difficulty: easy
tags: array, hash-table, sorting
signature: int[] arrayRankTransform(int[] arr)
---
List the different values that occur in `arr` from smallest to largest,
writing each value once however often it occurs. The **rank** of a value
is its position in that list, counting from 1: the smallest value has rank
1, the next one up has rank 2, and so on.

Return an array that gives, for each element of `arr` in turn, the rank of
its value. It is as long as `arr`, so an empty `arr` gives an empty array.

Equal elements therefore share a rank, and a tie leaves no gap: in
`[5,5,8]` both 5s get rank 1 and the 8 gets rank 2, not 3.

@examples

## Constraints

- `0 <= arr.length <= 10^5`
- `-10^9 <= arr[i] <= 10^9`
