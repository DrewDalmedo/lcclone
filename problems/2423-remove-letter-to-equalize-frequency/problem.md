id: 2423
slug: remove-letter-to-equalize-frequency
title: Remove Letter To Equalize Frequency
difficulty: easy
tags: hash-table, string, counting
signature: bool equalFrequency(string word)
---
Call a string **level** when all the letters that occur in it occur the
same number of times. Letters that don't occur at all are left out: `"xyy"`
is not level, but `"xxyy"` is, and so is any string of a single letter, such
as `"y"` or `"yyy"`.

Exactly one character of `word` has to go: pick a position and erase the
character there. Leaving `word` as it is is not allowed, even when it is
already level. If the erased character was the only copy of its letter,
that letter is gone from the string and plays no further part.

Return `true` if some choice of position leaves a level string, and
`false` if every choice leaves a string that is not level.

@examples

## Constraints

- `2 <= word.length <= 100`
- `word` consists of lowercase English letters.
