id: 1356
slug: sort-integers-by-the-number-of-1-bits
title: Sort Integers by The Number of 1 Bits
difficulty: easy
tags: bit-manipulation, array, counting, sorting
signature: int[] sortByBits(int[] arr)
---
The **bit count** of a non-negative integer is how many `1`s appear when
it is written in binary. For instance, `13` is `1101` in binary, so its bit
count is `3`; the bit count of `0` is `0`.

Return the values of `arr` rearranged by bit count, from the smallest bit
count to the largest. Values that share a bit count come in increasing
order among themselves. Nothing is added or dropped: a value that appears
three times in `arr` appears three times in the result.

@examples

## Constraints

- `1 <= arr.length <= 500`
- `0 <= arr[i] <= 10^4`
