id: 1704
slug: determine-if-string-halves-are-alike
title: Determine if String Halves Are Alike
difficulty: easy
tags: string, counting
signature: bool halvesAreAlike(string s)
---
`s` is a string of letters with an even length `n`. Cut it exactly in the
middle: the left piece is its first `n / 2` characters and the right piece
is the remaining `n / 2`.

In this problem the letters `a`, `e`, `i`, `o` and `u` are vowels, whether
they are written in lowercase or as capitals. Every other letter, `y` and
`Y` included, is a consonant.

Return `true` if the left piece and the right piece hold the same number of
vowels, and `false` if they don't.

Each occurrence counts once, so a piece containing `e` three times has three
vowels. Only the two totals are compared: it makes no difference which
vowels they are, what case they are in, or where they sit inside a piece.

@examples

## Constraints

- `2 <= s.length <= 1000`
- `s.length` is even.
- `s` consists of uppercase and lowercase English letters only.
