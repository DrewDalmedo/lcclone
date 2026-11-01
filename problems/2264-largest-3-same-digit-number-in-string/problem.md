id: 2264
slug: largest-3-same-digit-number-in-string
title: Largest 3-Same-Digit Number in String
difficulty: easy
tags: string
signature: string largestGoodInteger(string num)
---
`num` is a long number written out as a string of decimal digits. It may
begin with `0`. A **triple** is a block of three adjacent characters of
`num` that all hold the same digit, such as `"444"` or `"000"`.

Return the triple with the largest value, as a string of its three
characters. If `num` contains no triple at all, return the empty string
`""`.

Every triple is one digit written three times, so the triple of the highest
digit wins. `"000"` counts as a triple, with value zero, and it is returned
just as it appears: three zeros. A stretch of more than three equal digits,
such as `"8888"`, simply holds overlapping triples, all of them `"888"`.

The three digits have to be adjacent. In `"7177"` the digit `7` occurs three
times, but no three consecutive characters are all `7`, so there is no
triple.

@examples

## Constraints

- `3 <= num.length <= 1000`
- `num` consists of the digits `0` to `9` only.
