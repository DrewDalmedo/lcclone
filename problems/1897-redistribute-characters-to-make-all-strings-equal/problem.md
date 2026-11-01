id: 1897
slug: redistribute-characters-to-make-all-strings-equal
title: Redistribute Characters to Make All Strings Equal
difficulty: easy
tags: hash-table, string, counting
signature: bool makeEqual(string[] words)
---
`words` is a list of `n` strings made of lowercase letters. A **transfer**
lifts one letter out of one of the strings and drops it into another one
(never back into the string it came from), wherever you like: before its
first letter, after its last, or between two neighbours. A string can give
up a letter only while it still has one, but giving away its last letter
and being left empty is fine.

Transfers can be repeated as often as you want, or not used at all. Can
they bring every string in `words` to one and the same string, letter for
letter (equal lengths, and equal letters at every position)? Return `true`
if they can and `false` if they can't.

When `n = 1` there is no other string to transfer to, but the lone string
already agrees with itself, so the answer is `true`.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 100`
- `words[i]` consists of lowercase English letters.
