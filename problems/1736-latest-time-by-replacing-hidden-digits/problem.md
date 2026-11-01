id: 1736
slug: latest-time-by-replacing-hidden-digits
title: Latest Time by Replacing Hidden Digits
difficulty: easy
tags: greedy, string
signature: string maximumTime(string time)
---
A digital clock shows the time of day as `"HH:MM"` on a 24-hour dial: two
digits for the hour, a colon, then two digits for the minute, with leading
zeros (nine minutes past seven in the morning is `"07:09"`). A time is
**valid** when its hour is from `00` to `23` and its minute from `00` to
`59`, so the earliest time of the day is `"00:00"` and the latest
`"23:59"`.

Some of the four digits in `time` can't be read, and show as `?` instead.
Fill in each `?` with a digit, leaving the readable digits and the colon
alone, in such a way that the clock ends up showing a valid time. Among all
such fillings, pick the one that is furthest into the day, and return it in
the same five-character `"HH:MM"` form. If `time` has no `?`, return it
unchanged.

@examples

## Constraints

- `time.length == 5`
- `time[2]` is `':'`, and every other character of `time` is a digit or
  `'?'`.
- At least one way of replacing the `?` characters gives a valid time.
