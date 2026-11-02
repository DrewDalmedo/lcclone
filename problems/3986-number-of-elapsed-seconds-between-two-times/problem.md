id: 3986
slug: number-of-elapsed-seconds-between-two-times
title: Number of Elapsed Seconds Between Two Times
difficulty: easy
tags: math, string
signature: int secondsBetweenTimes(string startTime, string endTime)
---
A stopwatch is started when a 24-hour clock reads `startTime`, and it is
stopped when that clock reads `endTime`. Midnight never comes in between,
and the stopwatch may even be stopped the instant it was started. Return
what the stopwatch shows then, as a whole number of seconds.

Each reading is eight characters long, `"HH:MM:SS"`: the hour `HH` (`00` to
`23`), a colon, the minutes past the hour `MM` (`00` to `59`), a colon, and
the seconds past the minute `SS` (`00` to `59`). Every field has two
digits, so a value below 10 is written with a leading `0`: five seconds
past 7:04 in the morning is `"07:04:05"`, and the last second of the day is
`"23:59:59"`.

@examples

## Constraints

- `startTime.length == endTime.length == 8`
- `startTime` and `endTime` are laid out as `"HH:MM:SS"`, with digits
  everywhere but the colons at indices `2` and `5`.
- `00 <= HH <= 23`, `00 <= MM <= 59` and `00 <= SS <= 59` in both.
- `endTime` is not earlier than `startTime`.
