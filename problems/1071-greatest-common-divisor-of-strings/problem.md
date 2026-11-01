id: 1071
slug: greatest-common-divisor-of-strings
title: Greatest Common Divisor of Strings
difficulty: easy
tags: math, string
signature: string gcdOfStrings(string str1, string str2)
---
Picture a rubber stamp that prints a fixed non-empty word `x`. Pressing it
`k` times along a line, each print starting exactly where the previous one
ended, gives `x` repeated `k` times. The stamp **can print** a string `s`
when some `k >= 1` gives exactly `s`, so a stamp of `s` itself always can
(with `k = 1`).

Return the longest word `x` whose stamp can print both `str1` and `str2`.
When no stamp can print both, return the empty string `""`.

@examples

## Constraints

- `1 <= str1.length, str2.length <= 1000`
- `str1` and `str2` consist of uppercase English letters.
