id: 1360
slug: number-of-days-between-two-dates
title: Number of Days Between Two Dates
difficulty: easy
tags: math, string
signature: int daysBetweenDates(string date1, string date2)
---
`date1` and `date2` are calendar dates, each written as ten characters
in the order year, month, day: a four-digit year, a dash, a two-digit
month, another dash and a two-digit day. Single-digit months and days are
zero-padded, so the 7th of March 2051 is `"2051-03-07"`.

Return how many days apart the two dates are: the number of steps of one
day it takes to go forward from the earlier date to the later one. Either
argument may be the earlier date, and the answer is never negative. Two
dates in a row are `1` day apart, and a date is `0` days from itself.

The dates follow the Gregorian calendar. Its months have these lengths:
January 31, February 28 (29 in a leap year), March 31, April 30, May 31,
June 30, July 31, August 31, September 30, October 31, November 30 and
December 31. A year is a **leap year** when its number is a multiple of 4,
except that a year ending in `00` must be a multiple of 400 as well. So
among the years allowed here, 2000 has a 29th of February but 2100 does not.

@examples

## Constraints

- `date1.length == date2.length == 10`
- Both strings follow the layout above: digits, with a dash at indices `4`
  and `7`.
- Both are real dates from `"1971-01-01"` to `"2100-12-31"`, both
  included.
