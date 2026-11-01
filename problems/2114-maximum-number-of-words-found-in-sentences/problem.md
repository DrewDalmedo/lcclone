id: 2114
slug: maximum-number-of-words-found-in-sentences
title: Maximum Number of Words Found in Sentences
difficulty: easy
tags: array, string
signature: int mostWordsFound(string[] sentences)
---
Every string in `sentences` holds one sentence, written out neatly: words
made of lowercase letters, exactly one space between a word and the next,
and no space before the first word or after the last.

Find the sentence with the most words in it, and return its number of
words.

It is words that are counted, not letters: `"a b c"` has three words, so it
beats `"butterflies"`, which has one, although it is much shorter.

@examples

## Constraints

- `1 <= sentences.length <= 100`
- `1 <= sentences[i].length <= 100`
- `sentences[i]` consists of lowercase English letters and spaces.
- No sentence begins or ends with a space, and no two spaces are next to
  each other.
