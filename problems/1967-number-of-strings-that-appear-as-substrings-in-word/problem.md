id: 1967
slug: number-of-strings-that-appear-as-substrings-in-word
title: Number of Strings That Appear as Substrings in Word
difficulty: easy
tags: array, string
signature: int numOfStrings(string[] patterns, string word)
---
`word` is printed on a paper ribbon. A string **can be cut** from the
ribbon if you can snip out a piece of it that spells the string exactly:
a stretch of adjacent letters of `word`, in order, with none skipped. So
`"tea"` can be cut from `"steam"`, but `"sem"` can't, although its letters
appear in `"steam"` in that order. `word` itself can be cut from the
ribbon; a string longer than `word` never can.

Count the entries of `patterns` that can be cut from the ribbon, and return
that count. Each entry is tried on its own against the whole ribbon: the
cutting is only imagined, so nothing is used up. An entry that could be cut
from several places counts once, and an entry that appears several times in
`patterns` counts each time it appears.

@examples

## Constraints

- `1 <= patterns.length <= 100`
- `1 <= patterns[i].length <= 100`
- `1 <= word.length <= 100`
- `patterns[i]` and `word` consist of lowercase English letters.
