id: 1047
slug: remove-all-adjacent-duplicates-in-string
title: Remove All Adjacent Duplicates In String
difficulty: easy
tags: stack, string
signature: string removeDuplicates(string s)
---
`s` is a word of lowercase letters. Call two letters standing right next to
each other in it a **twin pair** when they are the same letter.

Cut a twin pair out of the word. The letters on its left and on its right
close the gap and become neighbours, which may create a new twin pair.
Repeat this, one twin pair at a time, until the word has none left, and
return what remains. It may be the empty string `""`.

Each cut takes exactly two letters: three equal letters in a row lose two
of them and keep one. The order in which the twin pairs are cut doesn't
change what remains in the end.

@examples

## Constraints

- `1 <= s.length <= 10^5`
- `s` consists of lowercase English letters only.

## Follow-up

Cutting one pair and then searching the word again from the start can take
`O(n^2)` time. Can you find what remains in a single pass from left to
right?
