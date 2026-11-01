id: 2506
slug: count-pairs-of-similar-strings
title: Count Pairs Of Similar Strings
difficulty: easy
tags: bit-manipulation, array, hash-table, string, counting
signature: int similarPairs(string[] words)
---
The **alphabet** of a word is the set of different letters it is spelled
with. It ignores how often each letter occurs and in what order, so
`"needed"` and `"den"` have the same alphabet, {d, e, n}. Call two words
**similar** when their alphabets are equal: every letter used in either
word is used in the other one too.

Return how many ways there are to choose two indices `i < j` so that
`words[i]` and `words[j]` are similar. Two copies of one word, at
different indices, also make such a pair.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 100`
- `words[i]` consists of lowercase English letters only.
