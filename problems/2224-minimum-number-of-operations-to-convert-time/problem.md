id: 2224
slug: minimum-number-of-operations-to-convert-time
title: Minimum Number of Operations to Convert Time
difficulty: easy
tags: greedy, string
signature: int convertTime(string current, string correct)
---
A wall clock shows the time `current`, but the right time is `correct`,
which is the same moment or later on the same day. The clock can only be
set forward, with four buttons: one press moves it ahead by 1, 5, 15 or 60
minutes, depending on the button. Any button may be pressed any number of
times, and there is no way to move the clock back.

Return the smallest total number of presses that makes the clock show
`correct`.

Each time is a five-character string `"HH:MM"` in 24-hour form, such as
`"07:45"` or `"19:03"`: `HH` (`00` to `23`) counts the hours since midnight
and `MM` (`00` to `59`) the minutes past the hour, and both always have two
digits.

@examples

## Constraints

- `current.length == correct.length == 5`
- `current` and `correct` are valid times in the `"HH:MM"` form described
  above.
- `current` is not later than `correct`.
