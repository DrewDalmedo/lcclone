id: 696
slug: count-binary-substrings
title: Count Binary Substrings
difficulty: easy
tags: two-pointers, string
signature: int countBinarySubstrings(string s)
---
`s` is a string of the digits `0` and `1`. Call a substring of `s` (one or
more consecutive characters of `s`) **half-and-half** when it can be cut in
the middle into two halves of the same length, one made only of `0`s and the
other made only of `1`s, in either order.

So `"10"`, `"0011"` and `"111000"` are half-and-half. `"0110"` and `"0101"`
are not: they hold as many `0`s as `1`s, but each half mixes both digits.
`"001"` (odd length) and `"11"` (no `0`) are not half-and-half either.

Return how many half-and-half substrings `s` has. They are counted by
position: when the same text appears at several places in `s`, each place
counts once.

@examples

## Constraints

- `1 <= s.length <= 10^5`
- Each `s[i]` is `'0'` or `'1'`.

## Follow-up

An `s` of length `n` has about `n^2 / 4` substrings of even length, far too
many to try one by one when `n` is large. Can you find the count in `O(n)`
time with `O(1)` extra memory?
