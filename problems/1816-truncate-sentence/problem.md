id: 1816
slug: truncate-sentence
title: Truncate Sentence
difficulty: easy
tags: array, string
signature: string truncateSentence(string s, int k)
---
A phone's lock screen has room for only part of a message, so it shows
just the message's first `k` words.

The message `s` is a **sentence**: a row of one or more words, each
spelled with English letters (`a`-`z` and `A`-`Z`, in any combination).
Each pair of adjacent words has one space between them, and `s` neither
starts nor ends with a space.

Return the preview, which keeps the first `k` words of `s`, in order and
spaced just as they are in `s`. Put another way, the preview is the part
of `s` in front of its `k`-th space; if `k` equals the number of words,
`s` has no `k`-th space and the preview is the whole of `s`. Copy the
letters unchanged: capitals stay capitals and small letters stay small.

@examples

## Constraints

- `1 <= s.length <= 500`
- `s` consists of English letters (`a`-`z`, `A`-`Z`) and spaces `' '`.
- No space at the start or end of `s`, and exactly one space between
  adjacent words.
- `1 <= k <= w`, where `w` is the number of words in `s`
