id: 1455
slug: check-if-a-word-occurs-as-a-prefix-of-any-word-in-a-sentence
title: Check If a Word Occurs As a Prefix of Any Word in a Sentence
difficulty: easy
tags: two-pointers, string, string-matching
signature: int isPrefixOfWord(string sentence, string searchWord)
---
A text editor completes words as you type. It has the line `sentence`, made
of words of lowercase letters with a single space between each word and the
next, and you have typed `searchWord`. The editor offers the first word of
`sentence`, reading from the left, that begins with what you typed.

A word **begins with** `searchWord` when its first `searchWord.length`
letters spell `searchWord` exactly. So the word can't be shorter than
`searchWord`, and a word equal to `searchWord` begins with it. Where else
`searchWord` turns up inside a word doesn't matter: `"bin"` is inside
`"cabin"`, but `"cabin"` doesn't begin with it.

The words of `sentence` are numbered from the left, starting at `1`.
Return the number of the word the editor offers, or `-1` if no word begins
with `searchWord`.

@examples

## Constraints

- `1 <= sentence.length <= 100`
- `1 <= searchWord.length <= 10`
- `sentence` consists of lowercase English letters and spaces.
- `searchWord` consists of lowercase English letters.
- `sentence` doesn't begin or end with a space, and its words are separated
  by single spaces.
