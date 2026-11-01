id: 2089
slug: find-target-indices-after-sorting-array
title: Find Target Indices After Sorting Array
difficulty: easy
tags: array, binary-search, sorting
signature: int[] targetIndices(int[] nums, int target)
---
Picture the elements of `nums` rearranged from smallest to largest, so that
each one is no larger than the next. Some positions of that sorted
arrangement hold the value `target`. Return those positions, numbered from
`0`, in increasing order. If no element of `nums` equals `target`, return
an empty array.

Equal elements can't be told apart, so the positions are the same however
ties are arranged.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i], target <= 100`
