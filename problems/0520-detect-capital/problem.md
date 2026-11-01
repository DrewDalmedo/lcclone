id: 520
slug: detect-capital
title: Detect Capital
difficulty: easy
tags: string
signature: bool detectCapitalUse(string word)
---
Every letter of `word` is either a capital (`A` to `Z`) or a small letter
(`a` to `z`). Three styles of writing a word are accepted:

- **lower**: `word` holds no capital;
- **upper**: `word` holds no small letter;
- **title**: `word[0]` is a capital, and every letter after it is small.

Return `true` if `word` is written in one of these three styles, and
`false` for any other mix of capitals and small letters.

The styles can't be combined within one word. So a word that begins with
two capitals has to be made of capitals only, and a word whose first letter
is small has to be made of small letters only. A word of a single letter
always fits, whichever its case.

@examples

## Constraints

- `1 <= word.length <= 100`
- Every character of `word` is an English letter, `a`-`z` or `A`-`Z`.
