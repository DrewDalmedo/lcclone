id: 2651
slug: calculate-delayed-arrival-time
title: Calculate Delayed Arrival Time
difficulty: easy
tags: math
signature: int findDelayedArrivalTime(int arrivalTime, int delayedTime)
---
A ferry is timetabled to dock at hour `arrivalTime` of a 24-hour clock, but
it runs `delayedTime` whole hours late. Return the hour, on the same clock,
at which it actually docks.

The clock shows the hours `0` to `23`, and the hour after `23` is `0`
(midnight). It does not keep track of days: a delay that carries the ferry
past midnight just wraps around, so the answer is always between `0` and
`23`.

@examples

## Constraints

- `1 <= arrivalTime <= 23`
- `1 <= delayedTime <= 24`
