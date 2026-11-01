id: 2027
slug: minimum-moves-to-convert-string
title: Minimum Moves to Convert String
difficulty: easy
tags: greedy, string
signature: int minimumMoves(string s)
---
The string `s`, of length `n`, uses just two letters: `'X'` and `'O'`
(the capital letter O, not the digit zero).

A **stamp** covers three neighbouring characters of `s` and turns each of
them into `'O'`. It must cover three positions that all lie inside `s`:
`k`, `k + 1` and `k + 2` for some `k` from `0` to `n - 3`. An `'O'` under
a stamp stays `'O'`, and a stamp may overlap earlier ones.

Return the smallest number of stamps after which no `'X'` is left in `s`.
If `s` has no `'X'` to begin with, the answer is `0`.

@examples

## Constraints

- `3 <= s.length <= 1000`
- `s[i]` is `'X'` or `'O'`.
