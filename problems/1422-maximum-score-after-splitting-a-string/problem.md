id: 1422
slug: maximum-score-after-splitting-a-string
title: Maximum Score After Splitting a String
difficulty: easy
tags: string, prefix-sum
signature: int maxScore(string s)
---
`s` is a row of the digits `0` and `1`. Cut it once, between two
neighbouring characters, into a front piece and a back piece. Both pieces
must keep at least one character, so a string of length `n` can be cut in
`n - 1` places.

A cut earns one point for each `0` in the front piece and one point for
each `1` in the back piece. Return the most points that any single cut
earns.

@examples

## Constraints

- `2 <= s.length <= 500`
- Every character of `s` is `'0'` or `'1'`.
