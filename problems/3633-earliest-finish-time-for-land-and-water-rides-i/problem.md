id: 3633
slug: earliest-finish-time-for-land-and-water-rides-i
title: Earliest Finish Time for Land and Water Rides I
difficulty: easy
tags: greedy, array, two-pointers, binary-search, sorting
signature: int earliestFinishTime(int[] landStartTime, int[] landDuration, int[] waterStartTime, int[] waterDuration)
---
A fairground has `n` land rides and `m` water rides. Land ride `i` opens
for the day at time `landStartTime[i]`, and one go on it takes
`landDuration[i]` time units. Water ride `j` likewise opens at
`waterStartTime[j]` and takes `waterDuration[j]`.

You will go on exactly two rides: one land ride and one water ride, any
pair you like, one after the other. You choose which of the two comes
first. The rules are:

- No ride can be boarded before it opens. Once open, it stays open, so
  you may board it at any later time. Boarding at time `t` means you get
  off at `t + duration`.
- Getting from one ride to the other takes no time. If your second ride
  is already open when you get off the first, you board it at once; if
  not, you board it the moment it opens.

Over every choice of the two rides and their order, return the smallest
time at which your second ride can end.

@examples

## Constraints

- `n == landStartTime.length == landDuration.length`
- `m == waterStartTime.length == waterDuration.length`
- `1 <= n, m <= 100`
- `1 <= landStartTime[i], landDuration[i] <= 1000`
- `1 <= waterStartTime[j], waterDuration[j] <= 1000`
