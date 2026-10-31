id: 884
slug: uncommon-words-from-two-sentences
title: Uncommon Words from Two Sentences
difficulty: easy
tags: hash-table, string, counting
signature: string[] uncommonFromSentences(string s1, string s2)
compare: unordered 0
---
You are given two sentences, `s1` and `s2`. Each is made of words of
lowercase letters, with a single space between one word and the next.

Put the words of both sentences together and count how many times each one
occurs. A word is **uncommon** when its count is exactly 1. So a word that
is repeated within a sentence, or that shows up in both sentences, is not
uncommon.

Return every uncommon word, each once, *in any order*. If there is none,
return an empty array.

Words are compared whole: `"an"`, `"ant"` and `"pant"` are three different
words.

@examples

## Constraints

- `1 <= s1.length, s2.length <= 200`
- `s1` and `s2` consist of lowercase English letters and spaces.
- Neither sentence begins or ends with a space, and their words are
  separated by single spaces.
