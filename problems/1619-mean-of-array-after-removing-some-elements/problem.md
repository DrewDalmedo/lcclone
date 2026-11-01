id: 1619
slug: mean-of-array-after-removing-some-elements
title: Mean of Array After Removing Some Elements
difficulty: easy
tags: array, sorting
signature: double trimMean(int[] arr)
---
The length `n` of `arr` is a multiple of `20`, so `5%` of it, `t = n / 20`,
is a whole number. Imagine `arr` sorted, then remove its `t` smallest
elements and its `t` largest elements. Return the average of the `n - 2t`
elements that remain.

Elements are removed one at a time, not by value: if the smallest value
occurs more than `t` times, only `t` of its copies are removed and the rest
stay, and the same goes for the largest value.

An answer within `10^-5` of the exact average is accepted.

@examples

## Constraints

- `20 <= arr.length <= 1000`
- `arr.length` is a multiple of `20`.
- `0 <= arr[i] <= 10^5`
