id: 35
slug: search-insert-position
title: Search Insert Position
difficulty: easy
tags: array, binary-search
signature: int searchInsert(int[] nums, int target)
---
`nums` is a list of distinct integers sorted from smallest to largest, and
`target` is an integer that may or may not appear in it.

Return the position where `target` belongs in `nums`, as an index counted
from `0`. When `nums` contains `target`, that is the index of `target`
itself. When it doesn't, it is the index of the first element larger than
`target`, or `nums.length` if no element is larger: the slot where `target`
could be added with `nums` still sorted.

Either way, the answer is the number of elements of `nums` that are smaller
than `target`.

@examples

## Constraints

- `1 <= nums.length <= 10^4`
- `-10^4 <= nums[i] <= 10^4`
- The values of `nums` are distinct and sorted in increasing order.
- `-10^4 <= target <= 10^4`

## Follow-up

Checking the elements one by one takes `O(n)` time. `nums` is sorted: can
you find the index in `O(log n)` time?
