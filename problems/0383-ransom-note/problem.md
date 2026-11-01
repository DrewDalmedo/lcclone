id: 383
slug: ransom-note
title: Ransom Note
difficulty: easy
tags: hash-table, string, counting
signature: bool canConstruct(string ransomNote, string magazine)
---
You want to paste together the message `ransomNote` out of letters cut
from the text `magazine`. A cut-out letter fills exactly one place in the
note and is then used up, so a note that uses `e` three times needs
`magazine` to hold at least three of them. Where a letter stood in
`magazine` doesn't matter, and letters you don't need are simply left
behind.

Return `true` if `magazine` has enough letters to make the whole note,
and `false` if some letter runs out first. Counting is all it takes: the
note can use a letter at most as many times as `magazine` contains it.

The two strings play different roles. A `magazine` with letters to spare
is fine, but the note can't be longer than `magazine`, or need a letter
it lacks.

@examples

## Constraints

- `1 <= ransomNote.length, magazine.length <= 10^5`
- `ransomNote` and `magazine` consist of lowercase English letters.
