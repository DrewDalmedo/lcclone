id: 977
slug: squares-of-a-sorted-array
title: Squares of a Sorted Array
difficulty: easy
tags: array, two-pointers, sorting
signature: int[] sortedSquares(int[] nums)
---
The integers in `nums` already stand in non-decreasing order, smallest
first, and some of them may be negative. Multiply each element by itself,
and return the squares, also arranged in non-decreasing order.

Each element contributes exactly one square, so the answer is as long as
`nums`. Repeated values give repeated squares, and so do opposite values
such as `-6` and `6`: every copy stays in the answer.

The order of `nums` doesn't carry over directly: a negative number far
below zero comes early in `nums`, yet its square can be the largest of all.

@examples

## Constraints

- `1 <= nums.length <= 10^4`
- `-10^4 <= nums[i] <= 10^4`
- `nums` is sorted in non-decreasing order.

## Follow-up

Can you build the answer in `O(n)` time, with no sort at all? Hint: the
biggest square has to come from the first or the last element of `nums`.
