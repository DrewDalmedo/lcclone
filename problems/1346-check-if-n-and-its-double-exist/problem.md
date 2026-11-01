id: 1346
slug: check-if-n-and-its-double-exist
title: Check If N and Its Double Exist
difficulty: easy
tags: array, hash-table, two-pointers, binary-search, sorting
signature: bool checkIfExist(int[] arr)
---
Look for a number in `arr` that is exactly twice some other number in
`arr`. Return `true` if there is one, and `false` if there is none.

The two numbers must sit at different places in `arr`, and either one may
come first. Negative numbers work the same way, so `-6` is twice `-3`.
Since `0` is twice itself, a `0` only counts when `arr` holds a second
`0`.

@examples

## Constraints

- `2 <= arr.length <= 500`
- `-10^3 <= arr[i] <= 10^3`
