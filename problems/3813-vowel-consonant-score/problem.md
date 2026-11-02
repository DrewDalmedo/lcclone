id: 3813
slug: vowel-consonant-score
title: Vowel-Consonant Score
difficulty: easy
tags: string, simulation
signature: int vowelConsonantScore(string s)
---
`s` is made of lowercase letters, digits and spaces. Go through it and
keep two tallies:

- `V` goes up by one for each character that is one of the five vowels
  `a`, `e`, `i`, `o`, `u`;
- `C` goes up by one for each character that is any other letter; `y` is
  one of these.

Digits and spaces leave both tallies unchanged.

The score is the largest whole number `q` with `q * C <= V`: the number of
times `C` fits into `V`, with any remainder dropped. When `C` is `0`, the
score is `0`, however large `V` is. Return the score.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters, the digits `0` to `9`, and
  spaces.
- `s` may begin or end with spaces, may have several spaces in a row, and
  need not contain a letter at all.
