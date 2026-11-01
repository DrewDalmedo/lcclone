id: 1588
slug: sum-of-all-odd-length-subarrays
title: Sum of All Odd Length Subarrays
difficulty: easy
tags: array, math, prefix-sum
signature: int sumOddLengthSubarrays(int[] arr)
---
A **window** of `arr` is a non-empty run of consecutive elements
`arr[l], arr[l + 1], ..., arr[r]`. Go through every window whose number of
elements is odd (1, 3, 5, ...), add up the elements of each one, and return
the grand total of those window sums.

A window is identified by where it starts and ends, so two windows in
different places count separately even when they hold the same values.

@examples

## Constraints

- `1 <= arr.length <= 100`
- `1 <= arr[i] <= 1000`

## Follow-up

Adding up every odd window one by one takes `O(n^3)` time. Can you find the
total in `O(n)` time, by working out how many odd windows contain each
element?
