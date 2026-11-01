id: 1534
slug: count-good-triplets
title: Count Good Triplets
difficulty: easy
tags: array, enumeration
signature: int countGoodTriplets(int[] arr, int a, int b, int c)
---
`a`, `b` and `c` are three closeness limits. A **good triplet** of `arr` is
a set of three positions, written from left to right as `i < j < k`, whose
values are close in every pair, each pair with its own limit:

- `arr[i]` and `arr[j]` differ by at most `a`;
- `arr[j]` and `arr[k]` differ by at most `b`;
- `arr[i]` and `arr[k]` differ by at most `c`.

"Differ by at most `a`" means the larger value minus the smaller one is `a`
or less, so a difference equal to the limit is allowed.

Return how many good triplets there are. Triplets are told apart by their
positions: two triplets with the same values at different positions both
count.

@examples

## Constraints

- `3 <= arr.length <= 100`
- `0 <= arr[i] <= 1000`
- `0 <= a, b, c <= 1000`
