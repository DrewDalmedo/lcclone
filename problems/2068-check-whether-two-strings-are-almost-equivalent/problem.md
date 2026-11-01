id: 2068
slug: check-whether-two-strings-are-almost-equivalent
title: Check Whether Two Strings are Almost Equivalent
difficulty: easy
tags: hash-table, string, counting
signature: bool checkAlmostEquivalent(string word1, string word2)
---
A **tally** of a word is a list of 26 counts, one per letter of the
alphabet: the count for a letter says how many of the word's letters are
that letter (`0` when there are none). Take the tallies of `word1` and of
`word2`, which have the same length, and compare them letter by letter.

The two words are **almost equivalent** when, for all 26 letters, the two
counts are at most `3` apart, whichever word holds more of the letter. So a
letter counted 7 times in one word and 4 in the other is fine, while one
counted 2 times and 6 times is not. Only the tallies matter, not where the
letters stand.

Return `true` if the two tallies are that close for every letter, and
`false` if some letter's counts are `4` or more apart.

@examples

## Constraints

- `word1.length == word2.length`
- `1 <= word1.length <= 100`
- `word1` and `word2` consist of lowercase English letters.
