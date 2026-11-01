id: 1796
slug: second-largest-digit-in-a-string
title: Second Largest Digit in a String
difficulty: easy
tags: hash-table, string
signature: int secondHighest(string s)
---
The string `s` mixes lowercase letters with the digits `0` to `9`. Skip the
letters and make a set of the digit values that show up in `s`: a digit is
in the set when it appears in `s` at all, and repeats add nothing to the
set. Where the digits sit in `s` doesn't matter.

Take the biggest value out of that set, and return the biggest value that
remains. If nothing remains, because `s` has no digit at all or every digit
in it is the same one, return `-1`.

@examples

## Constraints

- `1 <= s.length <= 500`
- `s` consists of lowercase English letters and digits only.
