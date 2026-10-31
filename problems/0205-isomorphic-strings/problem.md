id: 205
slug: isomorphic-strings
title: Isomorphic Strings
difficulty: easy
tags: hash-table, string
signature: bool isIsomorphic(string s, string t)
---
You are given two strings `s` and `t` of the same length. Decide whether
`t` is `s` written in a **substitution code**: a table that gives each
character occurring in `s` one code character, with different characters
always getting different code characters, such that replacing every
character of `s` by its code character, position by position, gives `t`.
Nothing stops the table from leaving a character unchanged.

Return `true` if such a table exists, and `false` if it doesn't.

Put another way: for any two positions `i` and `j`, the characters `s[i]`
and `s[j]` are equal exactly when `t[i]` and `t[j]` are equal.

Every character counts, not only letters: spaces, digits, punctuation and
control characters are characters like the rest. Characters are compared
by their exact code, so `a` and `A` are two different characters.

@examples

## Constraints

- `1 <= s.length <= 5 * 10^4`
- `t.length == s.length`
- `s` and `t` consist of ASCII characters other than NUL (codes 1 to 127).
