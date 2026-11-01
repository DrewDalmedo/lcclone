id: 1790
slug: check-if-one-string-swap-can-make-strings-equal
title: Check if One String Swap Can Make Strings Equal
difficulty: easy
tags: hash-table, string, counting
signature: bool areAlmostEqual(string s1, string s2)
---
`s1` and `s2` are strings of lowercase letters, both of the same length.
You may make at most one **exchange**: pick one of the two strings and two
of its positions, `i` and `j`, and let the letters at `i` and `j` change
places. Choosing `i == j` is allowed too; it leaves the string as it is.

Return `true` if the two strings can be made identical this way, and
`false` otherwise.

Strings that are already identical give `true`, since you need not
exchange anything. It makes no difference which string you pick:
exchanging positions `i` and `j` turns `s1` into `s2` exactly when it turns
`s2` into `s1`. You may not edit both strings, nor exchange twice
within the same one.

@examples

## Constraints

- `1 <= s1.length, s2.length <= 100`
- `s1.length == s2.length`
- `s1` and `s2` contain only lowercase English letters.
