id: 392
slug: is-subsequence
title: Is Subsequence
difficulty: easy
tags: two-pointers, string, dynamic-programming
signature: bool isSubsequence(string s, string t)
---
Suppose you may cross out characters of `t`, as many as you like, from
none of them to all of them, but may not move or change the ones you keep.
Return `true` if some choice of characters to cross out leaves exactly
`s`, read from left to right, and `false` if no choice does.

In other terms, the letters of `s` must be found in `t` in the same order,
though not necessarily side by side: each letter of `s` goes to a position
of `t` holding that same letter, and each next letter of `s` to a position
further right than the one before. A position of `t` serves at most one
letter of `s`, so a letter that occurs twice in `s` has to occur at least
twice in `t` too.

Crossing out everything leaves the empty string, so an empty `s` gives
`true`, even when `t` is empty as well.

@examples

## Constraints

- `0 <= s.length <= 100`
- `0 <= t.length <= 10^4`
- `s` and `t` consist of lowercase English letters.

## Follow-up

Now keep one `t` and test an enormous number of different strings `s`
against it, one after another. Scanning all of `t` for every `s` is then
too slow. What could you work out from `t` once, ahead of time, so that
each test costs about as many steps as `s` has letters (or a small factor
more), however long `t` is?
