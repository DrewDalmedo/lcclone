id: 434
slug: number-of-segments-in-a-string
title: Number of Segments in a String
difficulty: easy
tags: string
signature: int countSegments(string s)
---
The space character `' '` is the only separator in the string `s`. Cut
`s` at every space and throw the spaces away. That leaves pieces of text,
some of them empty (the piece between two spaces next to each other, for
one). Each piece that is not empty is a **segment**.

Return how many segments `s` has.

Every character other than the space ends up in some segment: letters,
digits and punctuation alike. So punctuation never splits a segment
(`"x,y"` is one segment), and a lone `"-"` with a space on each side is a
segment of its own. A string that is empty or holds only spaces has no
segments.

@examples

## Constraints

- `0 <= s.length <= 300`
- Each character of `s` is an English letter, a digit, a space `' '`, or
  one of the punctuation marks `!@#$%^&*()_+-=',.:`
