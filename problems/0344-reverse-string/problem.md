id: 344
slug: reverse-string
title: Reverse String
difficulty: easy
tags: two-pointers, string
signature: void reverseString(char[] s)
check: param s
---
`s` is an array of characters. Reverse it **in place**: the first character
swaps places with the last, the second with the second to last, and so on,
so that the character at index `i` ends up at index `s.length - 1 - i`.

The function returns nothing: `s` itself is checked after the call. The
examples show `s` as it must be afterwards.

@examples

## Constraints

- `1 <= s.length <= 10^5`
- Every `s[i]` is a printable ASCII character (codes 32 to 126), so spaces,
  quotes and backslashes may appear.

## Follow-up

Can you do it with `O(1)` extra memory, without a second array?
