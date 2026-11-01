id: 1185
slug: day-of-the-week
title: Day of the Week
difficulty: easy
tags: math
signature: string dayOfTheWeek(int day, int month, int year)
---
Three integers give a calendar date: `year`, `month` (from `1` for January
to `12` for December) and `day`, the day within that month. Work out which
day of the week that date is, and return its English name with a capital
first letter: one of `"Monday"`, `"Tuesday"`, `"Wednesday"`, `"Thursday"`,
`"Friday"`, `"Saturday"` and `"Sunday"`.

The calendar is the Gregorian one in everyday use. April, June, September
and November have 30 days; February has 28, or 29 in a **leap year**; the
other months have 31. The leap years are those divisible by `400`, together
with those divisible by `4` but not by `100`: 2000 is therefore a leap year,
but 2100 is not.

The weekdays repeat in the order listed above, day after day with no gaps,
and 1 January 2000 was a Saturday.

@examples

## Constraints

- `1971 <= year <= 2100`
- `1 <= month <= 12`
- `day` is a day that exists in that month of that year.
