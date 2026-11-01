id: 3280
slug: convert-date-to-binary
title: Convert Date to Binary
difficulty: easy
tags: math, string
signature: string convertDateToBinary(string date)
---
Some binary clocks show the date as well as the time. You are given
`date`, a day written as `"YYYY-MM-DD"` (so 9 March 1966 is
`"1966-03-09"`). Return the way such a clock spells it: the year number,
the month number and the day number, each converted from decimal to
binary, kept in that order and joined by single dashes.

Binary uses only the digits `0` and `1`. Reading from the right, its
places are worth 1, 2, 4, 8, 16 and so on, and a number is the total of
the places that hold a `1`. Write each of the three parts starting at its
highest `1`, with no zeros in front of it: the padding in `date` is not
carried over, so the month `03` becomes `"11"`, not `"011"` or `"0011"`.
None of the three numbers is ever 0, so every part begins with a `1`.

@examples

## Constraints

- `date.length == 10`
- `date[4] == date[7] == '-'`, and every other character of `date` is a
  digit.
- `date` is a valid date of the Gregorian calendar in one of the years
  `1900` to `2100`.
