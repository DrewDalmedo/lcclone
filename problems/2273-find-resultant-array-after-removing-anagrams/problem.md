id: 2273
slug: find-resultant-array-after-removing-anagrams
title: Find Resultant Array After Removing Anagrams
difficulty: easy
tags: array, hash-table, string, sorting
signature: string[] removeAnagrams(string[] words)
---
Two words are **anagrams** of each other when they are made of exactly the
same letters, repeats included, possibly in another order. `"stale"` and
`"least"` are anagrams. `"aab"` and `"abb"` are not, although both use only
`a` and `b`, and neither are `"ab"` and `"aabb"`. A word also counts as an
anagram of an identical copy of itself.

You are given a list `words` of lowercase words. Tidy it by repeating one
move for as long as it is possible: choose a word, other than the first,
that is an anagram of the word directly before it in the list as it
stands at that moment, and remove it. When no move is left, return the
words still in the list, spelled as they were and in their original order.

Which words you choose, and in which order, makes no difference to the list
that is left. Only neighbours matter: in `["ab","cd","ba"]` no word can be
removed, although `"ab"` and `"ba"` are anagrams.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 10`
- `words[i]` consists of lowercase English letters.
