id: 3794
slug: reverse-string-prefix
title: Reverse String Prefix
difficulty: easy
tags: two-pointers, string
signature: string reversePrefix(string s, int k)
---
The **prefix of length `k`** of `s` is its first `k` characters. Return a
copy of `s` in which that prefix is reversed, while every character at
index `k` or later stays at its index.

As a formula: for each index `i` below `k`, the result has `s[k - 1 - i]`
at index `i`, and at every other index it has the same character as `s`.
A one-letter prefix is its own reverse, so `k = 1` gives back `s` as it
was, and `k = s.length` gives all of `s` written backwards.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters.
- `1 <= k <= s.length`
