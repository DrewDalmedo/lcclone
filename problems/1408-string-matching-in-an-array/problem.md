id: 1408
slug: string-matching-in-an-array
title: String Matching in an Array
difficulty: easy
tags: array, string, string-matching
signature: string[] stringMatching(string[] words)
compare: unordered 0
---
`words` is a list of distinct words made of lowercase letters. A word of
the list **hides** in another word of the list when it appears there as a
run of consecutive letters. For example, `"ram"` hides in `"program"`, but
`"pgm"` does not: its letters are in `"program"`, but not next to each
other.

Return every word of `words` that hides in at least one other word of
`words`, *in any order*. A word that hides in several words is still
returned only once. If no word hides in another, return an empty array.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 30`
- `words[i]` consists of lowercase English letters only.
- All the words of `words` are distinct.
