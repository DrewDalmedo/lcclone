id: 4030
slug: check-ascii-palindromic
title: Check ASCII Palindromic
difficulty: easy
tags: bit-manipulation, two-pointers, string
signature: bool isPalindromic(string s)
---
Each lowercase letter has an ASCII code: `'a'` is `97`, `'b'` is `98`, and
so on up to `'z'`, which is `122`. Turn every letter of `s` into a **byte**:
its code written in binary with exactly 8 digits, the most significant
digit first, and zeros added on the left to fill up the 8 places. For
example, `'a'` becomes `01100001` and `'z'` becomes `01111010`.

Now write the bytes one after another, in the order of the letters of
`s`, as a single string of `8 * s.length` binary digits. Return `true` if
that string of digits is the same read backwards as read forwards, and
`false` otherwise.

The zeros added on the left are digits of the string like any other. And
only the string of digits matters: `s` itself may read the same both ways
while its digits do not, and the other way round.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters.
