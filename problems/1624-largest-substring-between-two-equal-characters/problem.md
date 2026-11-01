id: 1624
slug: largest-substring-between-two-equal-characters
title: Largest Substring Between Two Equal Characters
difficulty: easy
tags: hash-table, string
signature: int maxLengthBetweenEqualCharacters(string s)
---
Choose two different positions of the string `s` that hold the same letter.
The letters lying strictly between them form a **gap**. The two chosen
letters are not part of their gap, but anything else is allowed inside it,
other copies of that same letter included. A letter written twice in a row
leaves a gap of length `0`.

Return the length of the longest gap in `s`. If no letter occurs in `s` more
than once, there is no gap to measure: return `-1`.

@examples

## Constraints

- `1 <= s.length <= 300`
- `s` consists of lowercase English letters only.
