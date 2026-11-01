id: 3456
slug: find-special-substring-of-length-k
title: Find Special Substring of Length K
difficulty: easy
tags: string
signature: bool hasSpecialSubstring(string s, int k)
---
Look for `k` consecutive characters of `s` that are all one letter, say
`c`, and that are **fenced off**, with no other `c` touching them: right
before them comes the start of `s` or a letter different from `c`, and
right after them comes the end of `s` or a letter different from `c`.
Return `true` if `s` contains such a substring, and `false` if it contains
none.

Put another way, cut `s` into **blocks**, the longest pieces made of a
single repeated letter, so that `"ddkooo"` has the blocks `dd`, `k` and
`ooo`. The answer is `true` exactly when some block is `k` characters
long. A longer block doesn't count: wherever you take `k` of its letters,
another copy of the same letter sits right beside them.

@examples

## Constraints

- `1 <= k <= s.length <= 100`
- `s` consists of lowercase English letters.
