id: 1365
slug: how-many-numbers-are-smaller-than-the-current-number
title: How Many Numbers Are Smaller Than the Current Number
difficulty: easy
tags: array, hash-table, counting-sort, sorting
signature: int[] smallerNumbersThanCurrent(int[] nums)
---
Build an array `answer` with one entry per element of `nums`. Entry
`answer[i]` is the number of elements of `nums` whose value is strictly
below `nums[i]`; every element takes part, whether it sits before index `i`
or after it. Return `answer`.

Equal values are not below one another, so copies of one value never count
each other, and every copy of the smallest value gets `0`.

@examples

## Constraints

- `2 <= nums.length <= 500`
- `0 <= nums[i] <= 100`
