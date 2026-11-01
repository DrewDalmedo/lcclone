id: 2185
slug: counting-words-with-a-given-prefix
title: Counting Words With a Given Prefix
difficulty: easy
tags: array, string, string-matching
signature: int prefixCount(string[] words, string pref)
---
Lay the string `pref` over the left end of a word, letter on letter: the
first letter of `pref` on the first letter of the word, the second on the
second, and so on. The word **starts with** `pref` when every letter of
`pref` lands on the same letter. A word shorter than `pref` never does,
because part of `pref` lands on nothing, while a word spelled exactly like
`pref` does.

Only the left end of a word is tried, never a later position: `"plum"`
has `"lu"` at its second and third letters, so it doesn't start with
`"lu"`.

Return how many entries of `words` start with `pref`. Repeated entries are
not merged: if the same word sits at three positions of `words` and starts
with `pref`, it adds `3`.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length, pref.length <= 100`
- `words[i]` and `pref` consist of lowercase English letters only.
