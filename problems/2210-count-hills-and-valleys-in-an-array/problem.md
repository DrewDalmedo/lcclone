id: 2210
slug: count-hills-and-valleys-in-an-array
title: Count Hills and Valleys in an Array
difficulty: easy
tags: array
signature: int countHillValley(int[] nums)
---
Read `nums` from left to right as the heights along a trail, and cut it into
**plateaus**: maximal runs of adjacent equal elements. A plateau may be a
single element, and two plateaus next to each other always have different
heights. For example, `[5,5,2,9,9,9]` has three plateaus: `5,5`, then `2`,
then `9,9,9`.

A plateau is a **hill** when the plateaus on both sides of it are lower, and
a **valley** when they are both higher. A plateau with one neighbour lower
and the other higher is neither. The first and the last plateau have a
neighbour on one side only, so they are never hills or valleys.

Return the number of hills plus the number of valleys. A plateau counts
once, however many elements it has.

@examples

## Constraints

- `3 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
