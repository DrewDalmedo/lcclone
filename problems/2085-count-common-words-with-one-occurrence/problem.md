id: 2085
slug: count-common-words-with-one-occurrence
title: Count Common Words With One Occurrence
difficulty: easy
tags: array, hash-table, string, counting
signature: int countWords(string[] words1, string[] words2)
---
You are given two lists of lowercase words, `words1` and `words2`. A word
is **paired** when it occurs exactly once in `words1` and also exactly once
in `words2`.

Return how many different words are paired. A word that is repeated
within either list is not paired, even if the other list holds it only
once, and a word that is missing from one of the lists is not paired
either. Two words are the same only when they are identical strings, so
`"tea"` and `"team"` are different words.

@examples

## Constraints

- `1 <= words1.length, words2.length <= 1000`
- `1 <= words1[i].length, words2[j].length <= 30`
- `words1[i]` and `words2[j]` consist of lowercase English letters.
