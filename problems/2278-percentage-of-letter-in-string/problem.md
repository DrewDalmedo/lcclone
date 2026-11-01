id: 2278
slug: percentage-of-letter-in-string
title: Percentage of Letter in String
difficulty: easy
tags: string
signature: int percentageLetter(string s, char letter)
---
Let `c` be how many characters of `s` are equal to `letter`, and `n` the
length of `s`. Those `c` characters make up `100 * c / n` percent of the
string.

Return that percentage as a whole number by cutting off its fractional
part: the largest integer `p` with `p <= 100 * c / n`. It is never
rounded up, so a share of 87.5 percent gives `87`, not `88`.

`letter` doesn't have to occur in `s`; when it doesn't, the answer is
`0`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters.
- `letter` is a lowercase English letter.
