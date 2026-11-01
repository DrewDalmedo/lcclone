id: 3270
slug: find-the-key-of-the-numbers
title: Find the Key of the Numbers
difficulty: easy
tags: math
signature: int generateKey(int num1, int num2, int num3)
---
Write `num1`, `num2` and `num3` one below the other, lined up on the right
as for adding them by hand, so that units sit under units, tens under tens,
and so on. Each has at most four digits; fill the empty places on the left
of a shorter number with `0`s, so that all three are four digits wide and
form four columns: thousands, hundreds, tens and units.

The **key** of the three numbers has, in each column, the smallest of the
three digits in that column. Its four digits, taken from the thousands
column to the units column, make a number that may start with zeros.

Return the key as an integer, which drops any leading zeros: a key written
`0405` is returned as `405`, and a key of `0000` as `0`.

@examples

## Constraints

- `1 <= num1, num2, num3 <= 9999`
