id: 3083
slug: existence-of-a-substring-in-a-string-and-its-reverse
title: Existence of a Substring in a String and Its Reverse
difficulty: easy
tags: hash-table, string
signature: bool isSubstringPresent(string s)
---
Let `r` be the string `s` spelled backwards. A **link** of a string is any
two letters that stand next to each other in it, taken in the order they
appear: the links of `"stop"` are `"st"`, `"to"` and `"op"`, and the links
of its backwards spelling `"pots"` are `"po"`, `"ot"` and `"ts"`.

Return `true` if some link of `s` is also a link of `r`, and `false` if
none is.

Where the shared link sits in `s` and where it sits in `r` doesn't matter:
their positions need not correspond. It may even be made of the very same
two letters of `s`: a doubled letter such as `"oo"` is a link of `s`
and, read backwards, a link of `r` too. A string of one letter has no
links, so the answer for it is `false`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters.
