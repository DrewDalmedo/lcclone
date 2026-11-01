id: 1758
slug: minimum-changes-to-make-alternating-binary-string
title: Minimum Changes To Make Alternating Binary String
difficulty: easy
tags: string
signature: int minOperations(string s)
---
The string `s` is made of the digits `0` and `1`. Call such a string
**alternating** when no two characters that stand next to each other are
equal, as in `"1010"` or `"01010"`. A string of length 1 is alternating.

A change picks one position of `s` and replaces its digit with the other
one, turning a `0` into a `1` or a `1` into a `0`. Characters can't be
added, removed or moved, so the length of `s` stays the same.

Return the smallest number of changes after which `s` is alternating. It
may end up starting with either digit.

@examples

## Constraints

- `1 <= s.length <= 10^4`
- Each `s[i]` is `'0'` or `'1'`.
