id: 2351
slug: first-letter-to-appear-twice
title: First Letter to Appear Twice
difficulty: easy
tags: bit-manipulation, hash-table, string, counting
signature: char repeatedCharacter(string s)
---
Read the string `s` from left to right, one letter at a time. Stop at the
first letter that you have already read once before, and return it.

Put another way: among the letters that occur at least twice in `s`, return
the one whose **second** occurrence comes earliest. Where that letter first
occurs doesn't decide anything, and neither do any copies of it after the
second, nor its place in the alphabet.

Some letter is sure to occur twice in `s`, so there is always an answer.

@examples

## Constraints

- `2 <= s.length <= 100`
- `s` consists of lowercase English letters.
- At least one letter occurs two or more times in `s`.
