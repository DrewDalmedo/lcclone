id: 2437
slug: number-of-valid-clock-times
title: Number of Valid Clock Times
difficulty: easy
tags: string, enumeration
signature: int countTime(string time)
---
On a 24-hour scale, a time of day is written `"hh:mm"`: the hour `hh` is a
two-digit number from `00` to `23`, then comes a colon, then the minute
`mm`, a two-digit number from `00` to `59`. There are 1440 such times, from
`"00:00"` up to `"23:59"`; strings such as `"24:00"` or `"07:60"` are not
times.

The string `time` is a **pattern** of the same shape. Each of its four digit
places holds either a fixed digit or a `?`, and the colon is always in the
middle. A time of day **matches** the pattern when it has the same digit at
every place where the pattern has a fixed digit; at a `?` place any digit
will do, and different `?` places may take the same digit or different
ones.

Return how many times of day match `time`. A pattern without any `?`
matches exactly one time: itself.

@examples

## Constraints

- `time.length == 5`
- `time[2] == ':'`; the characters at indices 0, 1, 3 and 4 are digits
  or `'?'`.
- `time` comes from a valid time of day with some of its digits (perhaps
  none, perhaps all four) replaced by `'?'`, so at least one time matches.
