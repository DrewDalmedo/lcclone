id: 3216
slug: lexicographically-smallest-string-after-a-swap
title: Lexicographically Smallest String After a Swap
difficulty: easy
tags: greedy, string
signature: string getSmallestString(string s)
---
`s` is a string of decimal digits. You may change it by exchanging the
characters at two **neighbouring** positions `i` and `i + 1`, but only when
those two digits are both even (`0`, `2`, `4`, `6`, `8`) or both odd (`1`,
`3`, `5`, `7`, `9`). You may make this exchange **at most once**; making no
exchange at all is allowed too.

Among all the strings you can end up with, return the one that comes first
in lexicographic order: of two such strings, the smaller is the one with the
smaller digit at the first position where they differ.

The result is a string of the same length as `s`, not a number, so any
leading zeros stay in it.

@examples

## Constraints

- `2 <= s.length <= 100`
- `s` consists of digits `0` to `9` only.
