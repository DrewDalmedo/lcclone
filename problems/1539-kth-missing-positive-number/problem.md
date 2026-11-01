id: 1539
slug: kth-missing-positive-number
title: Kth Missing Positive Number
difficulty: easy
tags: array, binary-search
signature: int findKthPositive(int[] arr, int k)
---
The positive integers in `arr` are distinct and sorted from smallest to
largest. Count up through `1, 2, 3, ...`, skipping every number that
appears in `arr`; the numbers you do count are the **missing** ones. Stop
at the `k`-th missing number and return it.

Missing numbers never run out, because every number beyond the last
element of `arr` is missing too. So the answer always exists, and it may be
larger than every element of `arr`.

@examples

## Constraints

- `1 <= arr.length <= 1000`
- `1 <= arr[i] <= 1000`
- `1 <= k <= 1000`
- `arr[i] < arr[i + 1]` for `0 <= i < arr.length - 1`.

## Follow-up

Counting one number at a time takes time that grows with the answer. How
many numbers are missing below `arr[i]`? Can you use that to find the
answer in `O(log n)` time, where `n` is the length of `arr`?
