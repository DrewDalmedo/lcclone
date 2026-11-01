id: 1903
slug: largest-odd-number-in-string
title: Largest Odd Number in String
difficulty: easy
tags: greedy, math, string
signature: string largestOddNumber(string num)
---
`num` holds the decimal digits of a non-negative integer, most significant
digit first. It may run to `10^5` digits, so in general it can't be
converted into a built-in number type.

Cut a piece out of `num`: one or more digits that sit next to each other,
kept in their order. Read the piece as a decimal number; that number is
**odd** when the piece's last digit is `1`, `3`, `5`, `7` or `9`.

Among the pieces whose number is odd, return the one of greatest value,
written with its digits exactly as they appear in `num`. When there is no
odd piece at all (every digit of `num` is even), return `""`, the empty
string.

@examples

## Constraints

- `1 <= num.length <= 10^5`
- `num` consists of the digits `0` to `9` only.
- `num` has no leading zeros, though it may be `"0"` itself.
