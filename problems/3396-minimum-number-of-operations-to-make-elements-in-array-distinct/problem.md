id: 3396
slug: minimum-number-of-operations-to-make-elements-in-array-distinct
title: Minimum Number of Operations to Make Elements in Array Distinct
difficulty: easy
tags: array, hash-table
signature: int minimumOperations(int[] nums)
---
A conveyor belt carries a line of parcels, each stamped with a code. The
codes are listed front to back: `nums[0]` is on the parcel at the front of
the line, `nums[1]` on the one behind it, and so on.

A **sweep** takes the three parcels at the front off the belt. When fewer
than three parcels are left, a sweep takes all of them and the belt is
empty. Sweeps are the only way parcels leave, and the rest keep their order.

The belt is **clear** when no two parcels on it share a code. A belt with a
single parcel, or with none, is always clear.

Return the number of sweeps needed to make the belt clear: the smallest
`t >= 0` such that no code repeats among the parcels left after `t` sweeps.
If no code repeats to begin with, the answer is `0`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
