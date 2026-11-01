id: 1154
slug: day-of-the-year
title: Day of the Year
difficulty: easy
tags: math, string
signature: int dayOfYear(string date)
---
`date` names a day as `"YYYY-MM-DD"`: four digits of year, two of month and
two of day, with dashes between them. A month or day below 10 is written
with a leading zero, as in `"1975-04-09"`.

Number the days of each year in order, starting again from `1` every
January 1: January 2 is day `2`, February 1 is day `32`, and December 31 is
day `365`, or `366` in a leap year. Return the number that this gives to
`date`, counting within its own year.

Use the Gregorian calendar. From January to December the months have 31,
28, 31, 30, 31, 30, 31, 31, 30, 31, 30 and 31 days, except that February
has 29 in a leap year. Leap years are the multiples of 4, leaving out the
multiples of 100 that aren't also multiples of 400. Within the allowed
range, that makes every fourth year from 1904 to 2016 a leap year, 2000
included, while 1900 keeps a 28-day February.

@examples

## Constraints

- `date.length == 10`
- `date[4]` and `date[7]` are `'-'`, and every other character of `date` is
  a digit.
- `date` is a real calendar date, no earlier than `1900-01-01` and no later
  than `2019-12-31`.
