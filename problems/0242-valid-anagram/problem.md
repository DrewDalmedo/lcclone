id: 242
slug: valid-anagram
title: Valid Anagram
difficulty: easy
tags: hash-table, string, sorting
signature: bool isAnagram(string s, string t)
---
Decide whether the string `t` can be made by shuffling the string `s`: each
letter of `s` moves to some position of `t`, and no letter is added,
dropped or changed on the way. Return `true` if it can, and `false` if it
can't.

Put another way, the answer is `true` exactly when each letter from `a` to
`z` occurs as many times in `t` as it does in `s`. So two strings of
different lengths never qualify. A shuffle may leave every letter where it
is, so `t` may be equal to `s`.

@examples

## Constraints

- `1 <= s.length, t.length <= 5 * 10^4`
- `s` and `t` consist of lowercase English letters.

## Follow-up

A table of 26 counters is enough here because the alphabet is so small.
Suppose instead that `s` and `t` were UTF-8 text in which any Unicode
character could occur: how would you count the characters then?
