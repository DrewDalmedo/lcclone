id: 2446
slug: determine-if-two-events-have-conflict
title: Determine if Two Events Have Conflict
difficulty: easy
tags: array, string
signature: bool haveConflict(string[] event1, string[] event2)
---
Two events, `event1` and `event2`, are planned for the same day, and each
is given as a pair `[start, end]` of times. Times follow the 24-hour clock
and are written `"HH:MM"`, with exactly two digits on each side of the
colon: the hour `HH` is between `00` and `23`, and the minute `MM` between
`00` and `59`.

An event takes up its start, its end, and everything in between, with
**both ends included**; if its start and end are equal, it is a single
instant.

The two events **clash** when at least one point in time lies within both
of them. Touching is enough: an event that ends at `"10:30"` clashes with
one that starts at `"10:30"`. Return `true` if `event1` and `event2` clash,
and `false` if they don't.

@examples

## Constraints

- `event1.length == event2.length == 2`
- `event1[i].length == event2[i].length == 5`
- Every time is a valid `"HH:MM"` time as described above.
- In each event the start is not later than the end, so no event runs past
  midnight.
