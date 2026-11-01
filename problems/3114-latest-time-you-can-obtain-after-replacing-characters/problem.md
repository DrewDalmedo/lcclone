id: 3114
slug: latest-time-you-can-obtain-after-replacing-characters
title: Latest Time You Can Obtain After Replacing Characters
difficulty: easy
tags: string, enumeration
signature: string findLatestTime(string s)
---
The clock in this problem covers half a day. Its hour goes from 0 to 11
and its minute from 0 to 59, and a time is written with five characters,
`"HH:MM"`: two hour digits, a colon, then two minute digits, where a number
below 10 gets a zero in front (so 9 hours and 5 minutes is `"09:05"`). The
hour `00` counts and there is no hour `12`, so the clock starts at
`"00:00"` and stops at `"11:59"`.

`s` is shaped like such a time, but some of its four digit places are
blanks, each shown as `?`. Put a digit into every blank so that `s` reads
as a time on this clock. The blanks are filled independently (repeating a
digit is fine), and the digits already in `s` and its colon stay as they
are. Return the latest time you can get this way, written in the `"HH:MM"`
form. A string with no blank has a single filling: itself.

@examples

## Constraints

- `s.length == 5`
- `s[2]` is `':'`, and each of `s[0]`, `s[1]`, `s[3]` and `s[4]` is a
  digit or `'?'`.
- Some way of filling the blanks makes `s` a time from `"00:00"` to
  `"11:59"`.
