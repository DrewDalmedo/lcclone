id: 3833
slug: count-dominant-indices
title: Count Dominant Indices
difficulty: easy
tags: array, enumeration
signature: int dominantIndices(int[] nums)
---
Pick any index `i` of `nums` other than the last one, and gather the
elements to its right, from `nums[i + 1]` through the end of the array
(`nums[i]` itself is left out). Their **mean** is their sum divided by how
many of them there are, worked out exactly, with no rounding up or down:
the mean of `3` and `4` is `3.5`.

Index `i` is **dominant** when `nums[i]` is strictly greater than that
mean; being equal to it is not enough. The last index has nothing to its
right, so it is never dominant.

Return how many indices of `nums` are dominant.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
