id: 645
slug: set-mismatch
title: Set Mismatch
difficulty: easy
tags: bit-manipulation, array, hash-table, sorting
signature: int[] findErrorNums(int[] nums)
---
Write `n` for the number of elements of `nums`. For each integer `k` from
`1` to `n`, count the elements of `nums` equal to `k`. Every one of these
counts comes to `1` except two: one integer of that range, the
**doubled** one, is counted twice, and another, the **absent** one, is
counted zero times. So no element of `nums` lies outside `1` to `n`.
`nums` need not be sorted.

Return an array of two integers: the doubled one first, then the absent
one.

@examples

## Constraints

- `2 <= nums.length <= 10^4`
- `1 <= nums[i] <= 10^4`
- Every element of `nums` is between `1` and `nums.length`. Exactly one
  integer of that range occurs twice in `nums`, exactly one doesn't occur,
  and each of the others occurs once.

## Follow-up

A table of counts takes `O(n)` extra memory. Is there a way to get both
integers in linear time that keeps only a few variables besides the
result, and leaves `nums` as it is?
