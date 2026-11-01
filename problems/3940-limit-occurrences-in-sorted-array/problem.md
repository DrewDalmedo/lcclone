id: 3940
slug: limit-occurrences-in-sorted-array
title: Limit Occurrences in Sorted Array
difficulty: easy
tags: array, two-pointers
signature: int[] limitOccurrences(int[] nums, int k)
---
The integers in `nums` are sorted from smallest to largest, so all the
copies of any one value stand side by side in a single **block**. The
number `k` is a cap on how many copies of a value may remain.

Build a new array from `nums` by shortening every block that holds more
than `k` copies down to exactly `k` copies. A block of `k` copies or fewer
stays whole, so every value of `nums` still appears at least once. Nothing
else changes: the blocks keep their order, and the result is sorted as
well.

Return that array itself, with exactly the elements that remain, not just
its length.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
- `nums` is sorted in non-decreasing order.
- `1 <= k <= nums.length`

## Follow-up

Can you build the result in place, over the front of `nums`, with only
`O(1)` extra memory? The storage of the array you return doesn't count
toward that.
