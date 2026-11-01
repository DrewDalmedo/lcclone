id: 2255
slug: count-prefixes-of-a-given-string
title: Count Prefixes of a Given String
difficulty: easy
tags: array, string
signature: int countPrefixes(string[] words, string s)
---
Take the string `s` and cut any number of letters off its end, from none
up to all but one. What is left is a **prefix** of `s`. So a string of
length `k` has exactly `k` prefixes: its first letter, its first two
letters, and so on up to the whole string.

Return the number of indices `i` for which `words[i]` is a prefix of
`s`. Indices are counted, not distinct words: a word that occurs twice in
`words` and is a prefix of `s` is counted twice.

A prefix always starts at the first letter of `s`, so `"ell"` is not a
prefix of `"yellow"` although it occurs in it. A word longer than `s` is
never a prefix of it, even when it begins with all of `s`.

@examples

## Constraints

- `1 <= words.length <= 1000`
- `1 <= words[i].length, s.length <= 10`
- `words[i]` and `s` consist of lowercase English letters.
