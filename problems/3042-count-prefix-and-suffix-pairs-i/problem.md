id: 3042
slug: count-prefix-and-suffix-pairs-i
title: Count Prefix and Suffix Pairs I
difficulty: easy
tags: trie, array, string, string-matching, hash-function, rolling-hash
signature: int countPrefixSuffixPairs(string[] words)
---
Say that a word `a` **bookends** a word `b` when `b` starts with `a` and
also finishes with `a`. In other words, if `a` has `m` letters, then the
first `m` letters of `b` spell `a`, and so do the last `m` letters of `b`.

- The two copies of `a` may share letters: `"alfa"` bookends `"alfalfa"`,
  whose fourth letter belongs to both copies.
- Every word bookends itself.
- A word with more letters than `b` never bookends `b`.

You are given a list `words`. Return the number of pairs of positions
`(i, j)` with `i < j` such that `words[i]` bookends `words[j]`. Only the
word at the earlier position is tested as the bookend of the later one,
never the other way round. Pairs are counted by position: a word that
appears several times in the list forms a pair with each later copy of
itself.

@examples

## Constraints

- `1 <= words.length <= 50`
- `1 <= words[i].length <= 10`
- `words[i]` consists of lowercase English letters only.
