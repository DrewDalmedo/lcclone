id: 2908
slug: minimum-sum-of-mountain-triplets-i
title: Minimum Sum of Mountain Triplets I
difficulty: easy
tags: array
signature: int minimumSum(int[] nums)
---
Choose a left foot, a summit and a right foot among the elements of
`nums`, in that order from left to right: at indices `i < j < k`, which
don't have to be next to one another. The choice is a **mountain
triplet** when the summit is strictly higher than each foot, that is,
`nums[i] < nums[j]` and `nums[k] < nums[j]`. A foot
level with the summit doesn't count. The two feet are not compared with
each other: they may be equal, and either one may be the higher.

The cost of a mountain triplet is `nums[i] + nums[j] + nums[k]`. Return
the lowest cost of any mountain triplet in `nums`, or `-1` if there is no
mountain triplet at all.

@examples

## Constraints

- `3 <= nums.length <= 50`
- `1 <= nums[i] <= 50`
