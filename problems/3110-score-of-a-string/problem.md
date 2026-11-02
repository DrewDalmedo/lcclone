id: 3110
slug: score-of-a-string
title: Score of a String
difficulty: easy
tags: string
signature: int scoreOfString(string s)
---
Read the string `s` as a hiking route over hills. Every letter is a
checkpoint at some altitude given by its ASCII code, so `'a'` (97) is the
lowest possible checkpoint, `'b'` (98) is one step higher, and so on up to
`'z'` (122). The hiker starts at the first letter of `s` and walks straight
to each following letter in turn, ending at the last one.

Each leg of the walk, from one letter of `s` to the letter right after it,
costs the altitude gained or lost along it: from `'c'` up to `'h'` costs
`5`, from `'h'` down to `'c'` costs `5` as well, and between two equal
letters it costs `0`. The **score** of `s` is the total cost of all its
legs. The walk ends at the last letter: there is no leg from the last letter
back to the first.

Return the total cost of the whole walk, which is the score of `s`.

@examples

## Constraints

- `2 <= s.length <= 100`
- `s` consists of lowercase English letters only.
