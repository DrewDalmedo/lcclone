id: 3856
slug: trim-trailing-vowels
title: Trim Trailing Vowels
difficulty: easy
tags: string
signature: string trimTrailingVowels(string s)
---
Of the 26 lowercase letters, five are **vowels**: `a`, `e`, `i`, `o` and
`u`. The remaining 21, `y` among them, are **consonants**.

Find the last consonant in `s` and return everything up to and including
it: the vowels that follow it are cut off, no matter how many there are.
Letters before that consonant stay exactly as they are, vowels included,
so only the vowels at the very end of `s` go.

When the last letter of `s` is a consonant, nothing is cut and the answer
is `s` itself. When `s` contains no consonant, everything is cut and the
answer is the empty string `""`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters.
