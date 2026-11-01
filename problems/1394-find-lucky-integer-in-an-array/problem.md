id: 1394
slug: find-lucky-integer-in-an-array
title: Find Lucky Integer in an Array
difficulty: easy
tags: array, hash-table, counting
signature: int findLucky(int[] arr)
---
Take a number `v` that appears in `arr` and count its copies, the entries
of `arr` equal to `v`. The number is **lucky** when it has exactly `v`
copies, no more and no fewer: `4` is lucky if `arr` holds four 4s, but not
if it holds three or five.

Of all the lucky numbers in `arr`, return the greatest, judging by the
numbers themselves (not by their counts or where they sit). When `arr`
holds no lucky number at all, return `-1`.

@examples

## Constraints

- `1 <= arr.length <= 500`
- `1 <= arr[i] <= 500`
