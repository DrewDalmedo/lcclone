id: 2053
slug: kth-distinct-string-in-an-array
title: Kth Distinct String in an Array
difficulty: easy
tags: array, hash-table, string, counting
signature: string kthDistinct(string[] arr, int k)
---
An entry of `arr` is a **one-off** when no other entry of `arr` holds the
same string. A string written in `arr` twice or more gives no one-off at
all: none of its copies counts, the first one included. Two entries hold
the same string only when they match letter for letter over their full
length, so `"ab"`, `"ba"` and `"abc"` all differ from one another.

Go through `arr` from index `0` upward and number the one-offs `1`, `2`,
`3`, ... in the order you meet them. Return the string of the one-off
numbered `k`. When the numbering stops short of `k`, no one-off has that
number, and the answer is the empty string `""`.

@examples

## Constraints

- `1 <= k <= arr.length <= 1000`
- `1 <= arr[i].length <= 5`
- `arr[i]` consists of lowercase English letters.
