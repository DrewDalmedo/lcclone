id: 1446
slug: consecutive-characters
title: Consecutive Characters
difficulty: easy
tags: string
signature: int maxPower(string s)
---
Return the largest number of times the same letter occurs **in a row** in
the string `s`: the length of the longest substring of `s` whose characters
are all one letter, repeated.

The copies must sit side by side, with no other letter between them; call
such a stretch a **row**. In `"noon"` the two copies of `n` are not in one
row, so the answer is `2`, for `"oo"`. A single character is a row of
length 1 on its own, so the answer is never below `1`.

@examples

## Constraints

- `1 <= s.length <= 500`
- `s` consists of lowercase English letters.
