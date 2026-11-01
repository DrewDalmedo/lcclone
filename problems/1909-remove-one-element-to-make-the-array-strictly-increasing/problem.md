id: 1909
slug: remove-one-element-to-make-the-array-strictly-increasing
title: Remove One Element to Make the Array Strictly Increasing
difficulty: easy
tags: array
signature: bool canBeIncreasing(int[] nums)
---
A sequence is **strictly increasing** when each element is larger than the
one just before it; two equal neighbours already spoil it. A sequence of a
single element counts as strictly increasing.

Delete exactly one element of `nums`; everything after it moves one place
to the left, so the remaining elements stay in their original order. Return
`true` if some choice of the deleted element leaves a strictly increasing
array, and `false` if every choice fails.

If `nums` is strictly increasing to begin with, the answer is `true`:
deleting any one of its elements keeps it strictly increasing.

@examples

## Constraints

- `2 <= nums.length <= 1000`
- `1 <= nums[i] <= 1000`
