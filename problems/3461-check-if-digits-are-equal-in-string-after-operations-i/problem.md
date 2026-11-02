id: 3461
slug: check-if-digits-are-equal-in-string-after-operations-i
title: Check If Digits Are Equal in String After Operations I
difficulty: easy
tags: math, string, combinatorics, number-theory, simulation
signature: bool hasSameDigits(string s)
---
`s` is a string of decimal digits. **Folding** a string of `L` digits
turns it into a string of `L - 1` digits: the digit at position `j` of the
new string (counting from 0) is the last digit of the sum of the digits
at positions `j` and `j + 1` of the old one. For instance, `7` next to `5`
gives `2`, and `9` next to `9` gives `8`.

Fold `s`, then fold the result, and keep going until only two digits are
left; a string of three digits is folded just once. Return `true` if the
two digits left are the same, and `false` if they differ.

The digits are added as numbers from 0 to 9. A `0` at the front of `s` is
an ordinary digit like any other.

@examples

## Constraints

- `3 <= s.length <= 100`
- `s` consists only of the characters `'0'` to `'9'`.
