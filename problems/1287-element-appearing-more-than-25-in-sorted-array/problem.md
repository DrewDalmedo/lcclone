id: 1287
slug: element-appearing-more-than-25-in-sorted-array
title: Element Appearing More Than 25% In Sorted Array
difficulty: easy
tags: array
signature: int findSpecialInteger(int[] arr)
---
`arr` holds `n` integers in non-decreasing order, so all copies of a value
stand together in one block. Call a value **frequent** when its block is
longer than a quarter of the array, that is, when four times its number of
copies is greater than `n`.

Exactly one value of `arr` is frequent. Return it.

A block of exactly a quarter is not long enough. With `n = 12`, a value
with 3 copies is not frequent, and one with 4 copies is.

@examples

## Constraints

- `1 <= arr.length <= 10^4`
- `0 <= arr[i] <= 10^5`
- `arr` is sorted in non-decreasing order.
- Exactly one value appears more than `arr.length / 4` times.
