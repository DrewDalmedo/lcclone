id: 2848
slug: points-that-intersect-with-cars
title: Points That Intersect With Cars
difficulty: easy
tags: array, hash-table, prefix-sum
signature: int numberOfPoints(int[][] nums)
---
A street has a painted marker every metre along one kerb, and each marker
shows its distance in metres from the corner, the nearest one being marker
`1`. A parking warden logs every car that parks at that kerb during one
day: the entry `nums[i] = [a, b]` records a car that stood alongside each
marker from `a` to `b`, **both ends included**. When `a == b`, that car
stood alongside marker `a` alone.

Cars came and went at different times, so two entries may overlap, one may
lie entirely within another, two may meet at a single shared marker, and
the same entry may appear several times. The warden did not sort the log.

Return how many different markers had a car alongside them at some point
during the day. A marker that had several cars alongside it, at different
times, still counts once.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `nums[i].length == 2`
- `1 <= nums[i][0] <= nums[i][1] <= 100`
