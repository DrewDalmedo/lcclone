id: 1221
slug: split-a-string-in-balanced-strings
title: Split a String in Balanced Strings
difficulty: easy
tags: greedy, string, counting
signature: int balancedStringSplit(string s)
---
The string `s` is made of the letters `L` and `R`, and it has exactly as
many `L`s as `R`s.

You may cut `s` in any of the gaps between two neighbouring letters: in as
many gaps as you like, or in none. The cuts break `s` into pieces, and each
letter ends up in exactly one piece, in its original order. A choice of cuts
is **allowed** when every piece, just like `s` itself, has as many `L`s as
`R`s.

Return the largest number of pieces that an allowed choice of cuts
produces. Making no cut at all is always allowed, so the answer is at least
`1`.

@examples

## Constraints

- `2 <= s.length <= 1000`
- `s[i]` is `L` or `R`.
- `s` has as many `L`s as `R`s.
