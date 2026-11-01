id: 709
slug: to-lower-case
title: To Lower Case
difficulty: easy
tags: string
signature: string toLowerCase(string s)
---
The 26 capital letters `A` to `Z` each have a small partner, `a` to `z`
in the same order. Make a copy of `s` in which every capital has been
swapped for its partner, and return the copy.

No other character is touched. Small letters, digits, spaces and
punctuation are copied as they are, and so are the characters that lie
around and between the two runs of letters in the ASCII table, such as
`@`, `[`, `_` and `{`. So the copy has as many characters as `s`, in the
same order.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of printable ASCII characters (codes 32 to 126): letters,
  digits, spaces and punctuation.
