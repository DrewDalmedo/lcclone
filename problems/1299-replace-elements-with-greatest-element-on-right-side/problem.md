id: 1299
slug: replace-elements-with-greatest-element-on-right-side
title: Replace Elements with Greatest Element on Right Side
difficulty: easy
tags: array
signature: int[] replaceElements(int[] arr)
---
`arr` has `n` positive integers. Return an array `out` of the same length
in which `out[i]` is the largest value found **after** position `i`, that
is, the maximum of `arr[i + 1]`, ..., `arr[n - 1]`. The value at position
`i` is not part of that range. The last position has nothing after it, so
`out[n - 1]` is `-1`.

You may build `out` as a new array or write it over `arr` and return
`arr`.

@examples

## Constraints

- `1 <= arr.length <= 10^4`
- `1 <= arr[i] <= 10^5`

## Follow-up

Scanning the rest of the array for every position takes `O(n^2)` time.
Can you fill in `out` in a single pass?
