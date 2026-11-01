id: 3174
slug: clear-digits
title: Clear Digits
difficulty: easy
tags: stack, string, simulation
signature: string clearDigits(string s)
---
`s` is a mix of lowercase letters and digits, and every digit in it is an
**eraser**. Go through `s` from left to right. Each time you come to a
digit, it wipes out the nearest letter before it that hasn't been wiped
out yet, and then the digit itself disappears too. Letters are removed
only this way, one per digit.

Return the letters that survive, in the order they stand in `s`. If every
letter has been wiped out, return the empty string `""`.

What a digit is worth doesn't matter: `0` wipes out one letter, and so
does `9`. The input is such that each digit always finds a letter before
it to wipe out.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters and digits only.
- Every digit has a letter left to wipe out when its turn comes: in each
  prefix of `s`, there are at least as many letters as digits.
