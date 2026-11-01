id: 3184
slug: count-pairs-that-form-a-complete-day-i
title: Count Pairs That Form a Complete Day I
difficulty: easy
tags: array, hash-table, counting
signature: int countCompleteDayPairs(int[] hours)
---
`hours` lists the lengths of some jobs, in hours. Take two of them and run
them back to back, starting the first one at midnight. The two jobs make a
**complete day** when the second one finishes exactly at a midnight, which
happens when their lengths add up to a whole number of days: a multiple of
`24`, whether that is `24` itself, `48`, `240` or more.

Return how many pairs of jobs make a complete day. A pair is two different
positions `i < j` of `hours`, so a job is never paired with itself, and
equal lengths at different positions form pairs of their own.

@examples

## Constraints

- `1 <= hours.length <= 100`
- `1 <= hours[i] <= 10^9`
