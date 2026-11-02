id: 3823
slug: reverse-letters-then-special-characters-in-a-string
title: Reverse Letters Then Special Characters in a String
difficulty: easy
tags: two-pointers, string, simulation
signature: string reverseByType(string s)
---
Each character of `s` is either a lowercase letter or one of ten
**special characters**: `!`, `@`, `#`, `$`, `%`, `^`, `&`, `*`, `(` and
`)`. Rearrange `s` in two rounds:

1. Lift every letter out of `s`, leaving a gap where each one stood. Turn
   the row of lifted letters back to front, then drop them into the gaps,
   one per gap, from left to right.
2. Do the same with the special characters: lift them out, turn their row
   back to front, and drop them into their own gaps from left to right.

Return the string this gives. A round only fills the gaps it made, so a
letter always lands where a letter stood and a special character where a
special character stood. Characters are moved but never changed: a `(`
is still a `(` wherever it lands, not a `)`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters and the characters
  `!@#$%^&*()`. It may contain only letters, or only special characters.
