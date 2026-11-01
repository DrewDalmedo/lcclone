id: 504
slug: base-7
title: Base 7
difficulty: easy
tags: math, string
signature: string convertToBase7(int num)
---
Return the base-7 numeral for `num`, as a string.

Base 7 has seven digits, `0` to `6`, and each place is worth seven times
the place to its right: read from the right, the places count ones,
sevens, 49s, 343s, and so on. There are no leading zeros: a numeral
starts at the highest place that holds a nonzero digit. Zero, which has
no such place, is written `"0"`.

A negative `num` is written as in everyday decimal: a minus sign `-`
followed by the numeral for `-num`. A positive `num` has no sign.

@examples

## Constraints

- `-10^7 <= num <= 10^7`
