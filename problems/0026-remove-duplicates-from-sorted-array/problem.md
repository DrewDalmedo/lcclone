id: 26
slug: remove-duplicates-from-sorted-array
title: Remove Duplicates from Sorted Array
difficulty: easy
tags: array, two-pointers
signature: int removeDuplicates(int[] nums)
check: prefix nums
---
`nums` is sorted in non-decreasing order, so equal values sit next to each
other. Squeeze out the repeats **in place**: rearrange `nums` so that every
distinct value in it appears once at the front of the array, in increasing
order, and return how many distinct values there are.

If that count is `k`, only `nums[0]` to `nums[k - 1]` are checked, in order.
Whatever `nums` holds after them doesn't matter, and you don't need to
shrink it. The examples show those `k` elements as the output.

@examples

## Constraints

- `1 <= nums.length <= 3 * 10^4`
- `-100 <= nums[i] <= 100`
- `nums` is sorted in non-decreasing order.

## Follow-up

Can you do it in a single pass over `nums`, with `O(1)` extra memory?
