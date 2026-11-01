id: 1768
slug: merge-strings-alternately
title: Merge Strings Alternately
difficulty: easy
tags: two-pointers, string
signature: string mergeAlternately(string word1, string word2)
---
`word1` and `word2` take turns handing letters to a new string. On each
turn a word gives up exactly one letter, the first one it hasn't given
yet. `word1` has the first turn, even when it is the shorter of the two,
so while both words still have letters, the new string alternates between
them and begins with `word1[0]`.

As soon as one word has handed over its last letter, the turns end, and
the letters the other word still holds are added at the end, all at once
and in their original order. No letter is dropped, repeated or changed:
the new string is `word1.length + word2.length` letters long.

Return the new string.

@examples

## Constraints

- `1 <= word1.length, word2.length <= 100`
- `word1` and `word2` consist of lowercase English letters.
