id: 1566
slug: detect-pattern-of-length-m-repeated-k-or-more-times
title: Detect Pattern of Length M Repeated K or More Times
difficulty: easy
tags: array, enumeration
signature: bool containsPattern(int[] arr, int m, int k)
---
Return `true` if somewhere in `arr` the same `m` values occur `k` times in
a row, and `false` otherwise. In other words, look for an index `i` such
that the `m * k` elements starting at `arr[i]` form `k` consecutive groups
of `m`, and every group equals the first one, position by position.

All the groups must fit inside `arr`. Being consecutive, they neither
overlap nor leave gaps, so equal groups with other values between them do
not continue a repetition. The repetition may begin at any index, and more
than `k` equal groups in a row is fine too.

@examples

## Constraints

- `2 <= arr.length <= 100`
- `1 <= arr[i] <= 100`
- `1 <= m <= 100`
- `2 <= k <= 100`
