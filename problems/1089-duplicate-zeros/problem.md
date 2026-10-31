id: 1089
slug: duplicate-zeros
title: Duplicate Zeros
difficulty: easy
tags: array, two-pointers
signature: void duplicateZeros(int[] arr)
check: param arr
---
Change `arr` **in place** so that every zero in it is written twice. Read
the original `arr` from left to right and write the values back in that
order, each `0` two times and every other value once. So each value moves
one place to the right for every zero that came before it.

`arr` keeps its length, so whatever would land past its last index is
dropped. That includes the second copy of a zero whose first copy lands on
the last index. Only the zeros of the original `arr` are doubled, not the
copies.

The function returns nothing: `arr` itself is checked after the call. The
examples show `arr` as it must be afterwards.

@examples

## Constraints

- `1 <= arr.length <= 10^4`
- `0 <= arr[i] <= 9`

## Follow-up

Can you do it with `O(1)` extra memory, without a second array?
