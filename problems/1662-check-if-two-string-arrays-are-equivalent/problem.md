id: 1662
slug: check-if-two-string-arrays-are-equivalent
title: Check If Two String Arrays are Equivalent
difficulty: easy
tags: array, string
signature: bool arrayStringsAreEqual(string[] word1, string[] word2)
---
Glue the strings of `word1` together end to end, in array order and with
nothing put between them, to get one piece of text. Glue the strings of
`word2` together the same way. Return `true` if the two pieces of text are
identical, character for character, and `false` otherwise.

Only the glued text matters, not where it was cut: the two arrays may hold
different numbers of strings, and their strings may begin and end at
different places in the text.

@examples

## Constraints

- `1 <= word1.length, word2.length <= 1000`
- `1 <= word1[i].length, word2[i].length <= 1000`
- The strings of `word1` have at most `1000` characters in all, and so do
  the strings of `word2`.
- Every string consists of lowercase English letters.

## Follow-up

Can you decide it with `O(1)` extra memory, without building either piece
of text?
