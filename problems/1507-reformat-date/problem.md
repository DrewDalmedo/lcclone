id: 1507
slug: reformat-date
title: Reformat Date
difficulty: easy
tags: string
signature: string reformatDate(string date)
---
`date` is a date in the style of a letterhead, three parts with one space
between each:

1. the day of the month as an English ordinal: the number, with no
   leading zero, followed by `st`, `nd`, `rd` or `th`, as in `1st`, `2nd`,
   `3rd`, `4th`, `11th`, `22nd` or `30th`;
2. the month, shortened to its first three letters with only the first one
   a capital: `Jan`, `Feb`, `Mar`, `Apr`, `May`, `Jun`, `Jul`, `Aug`,
   `Sep`, `Oct`, `Nov` or `Dec`, which stand for months `1` to `12` in
   that order;
3. the year, four digits.

Return the same date in the all-numeric form that file names and
databases use: the year, a hyphen, the month's number, a hyphen, then the
day's number.
The month and the day always take two digits each, so `5` is written
`05`; the year keeps its four digits as given. The result is always ten
characters long.

@examples

## Constraints

- `date` is a real calendar date written as described above.
- The year is between `1900` and `2100`.
