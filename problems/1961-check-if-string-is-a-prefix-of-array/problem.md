id: 1961
slug: check-if-string-is-a-prefix-of-array
title: Check If String Is a Prefix of Array
difficulty: easy
tags: array, two-pointers, string
signature: bool isPrefixString(string s, string[] words)
---
A message reaches you in pieces: first `words[0]`, then `words[1]`, and so
on through the array. Each piece arrives whole and is added to the end of
the text received so far, with nothing in between.

You are waiting for the text `s`. Return `true` if, right after some piece
has arrived, the text received so far is exactly `s`, and `false`
otherwise.

What arrives after that moment doesn't matter: the message may go on past
`s`. But the text is only looked at between two pieces, never while one is
half received, so `s` must end exactly where a piece ends.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 20`
- `1 <= s.length <= 1000`
- `words[i]` and `s` consist of lowercase English letters.
