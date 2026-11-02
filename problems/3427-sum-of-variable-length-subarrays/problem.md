id: 3427
slug: sum-of-variable-length-subarrays
title: Sum of Variable Length Subarrays
difficulty: easy
tags: array, prefix-sum
signature: int subarraySum(int[] nums)
---
Each element of `nums` looks back along the array, as far as its own value
says. Element `i` sees itself and the `nums[i]` elements just before it,
`nums[i - nums[i]]` through `nums[i - 1]`. When fewer than `nums[i]`
elements come before it, it sees all of them, back to `nums[0]`. So it
sees a run of `min(i, nums[i]) + 1` consecutive elements, ending with
itself.

The **view** of element `i` is the sum of the elements it sees. Return the
sum of the views of all the elements. An element seen by several elements
counts once in each of their views.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 1000`
