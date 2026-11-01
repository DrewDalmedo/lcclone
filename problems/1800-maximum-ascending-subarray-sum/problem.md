id: 1800
slug: maximum-ascending-subarray-sum
title: Maximum Ascending Subarray Sum
difficulty: easy
tags: array
signature: int maxAscendingSum(int[] nums)
---
Pick two indices `l <= r` of `nums` and look at the slice `nums[l]`,
`nums[l + 1]`, ..., `nums[r]`. Call the slice **rising** when every value
in it except the last is smaller than the value that follows it in the
slice. A value followed by an equal one stops a slice from rising, and a
slice holding just one value is always rising.

Out of every rising slice of `nums`, return the largest total of its
values.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
