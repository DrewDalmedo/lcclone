id: 3028
slug: ant-on-the-boundary
title: Ant on the Boundary
difficulty: easy
tags: array, prefix-sum, simulation
signature: int returnToBoundaryCount(int[] nums)
---
An ant lives on a straight track with no ends. One point of the track is
painted: that point is the **boundary**, and the ant sets out from it.

The ant makes one hop for each value of `nums`, taking them first to last.
The hop for `nums[i]` covers `|nums[i]|` units: towards the right when
`nums[i]` is positive, towards the left when it is negative. No value is
`0`, so every hop moves the ant.

Count the hops that end with the ant standing exactly on the boundary, and
return that count. Only the landing spot of a hop matters: a hop that
carries the ant from one side of the boundary to the other, without
stopping on it, does not count. Neither does the ant's starting point,
before its first hop.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `-10 <= nums[i] <= 10`
- `nums[i] != 0`
