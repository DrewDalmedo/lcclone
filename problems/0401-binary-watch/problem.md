id: 401
slug: binary-watch
title: Binary Watch
difficulty: easy
tags: bit-manipulation, backtracking
signature: string[] readBinaryWatch(int turnedOn)
compare: unordered 0
---
A watch shows the time with 10 lights, each either on or off. Four lights
show the hour and six show the minute, each as a binary number: the hour
lights are worth 1, 2, 4 and 8, the minute lights 1, 2, 4, 8, 16 and 32,
and the hour (or minute) shown is the sum of its lights that are on.

The hour goes from 0 to 11 and the minute from 0 to 59; there is no light
for morning or afternoon. A pattern whose hour lights add up to 12 or more,
or whose minute lights add up to 60 or more, is **not** a time.

Given `turnedOn`, the number of lights that are on, return every time the
watch can show with exactly that many lights on. Each time appears once, and
the times may be returned *in any order*. If no time has that many lights
on, return an empty array.

Write each time as the hour in decimal with no leading zero, a colon, and
the minute as exactly two digits: 6 hours and 3 minutes is `"6:03"` (not
`"06:03"` or `"6:3"`), and midnight is `"0:00"`.

@examples

## Constraints

- `0 <= turnedOn <= 10`
