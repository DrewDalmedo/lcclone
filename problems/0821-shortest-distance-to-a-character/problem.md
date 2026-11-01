id: 821
slug: shortest-distance-to-a-character
title: Shortest Distance to a Character
difficulty: easy
tags: array, two-pointers, string
signature: int[] shortestToChar(string s, char c)
---
Picture someone standing on one letter of the string `s`, which is made of
lowercase letters. Each step moves them one letter to the left or one
letter to the right. The letter `c` appears in `s` at least once.

For each index `i`, count the fewest steps that take someone standing on
`s[i]` to a copy of `c`. That is the smallest `|i - j|` over all indices `j`
with `s[j] == c`, and it is `0` when `s[i]` is itself `c`. Return these
counts as an array of `s.length` integers, the count for index `0` first.

@examples

## Constraints

- `1 <= s.length <= 10^4`
- `s[i]` and `c` are lowercase English letters.
- `c` occurs in `s` at least once.
