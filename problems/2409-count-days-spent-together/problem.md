id: 2409
slug: count-days-spent-together
title: Count Days Spent Together
difficulty: easy
tags: math, string
signature: int countDaysTogether(string arriveAlice, string leaveAlice, string arriveBob, string leaveBob)
---
Alice and Bob each spend one stretch of the year at the same mountain
cabin: Alice from `arriveAlice` to `leaveAlice`, and Bob from `arriveBob`
to `leaveBob`. The day of arrival and the day of departure both count as
days spent at the cabin, so a stay from `"06-29"` to `"07-02"` covers four
days.

Return the number of days on which both guests are at the cabin. If their
stays have no day in common, return `0`.

Every date is written `"MM-DD"`: the month as two digits (`01` for January
up to `12` for December), a dash, then the day of the month as two digits,
with a leading zero below 10. The dates carry no year: they all fall in
the same one, which has 365 days and a 28-day February, so its months have
these lengths:

```text
month  01 02 03 04 05 06 07 08 09 10 11 12
days   31 28 31 30 31 30 31 31 30 31 30 31
```

@examples

## Constraints

- Every argument is exactly five characters long and follows the
  `"MM-DD"` layout above.
- Each names a day that exists in that year (so never `"02-29"` or
  `"04-31"`).
- Nobody leaves before arriving: `leaveAlice` is not earlier in the year
  than `arriveAlice`, and `leaveBob` is not earlier than `arriveBob`.
