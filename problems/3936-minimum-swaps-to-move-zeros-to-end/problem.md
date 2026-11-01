id: 3936
slug: minimum-swaps-to-move-zeros-to-end
title: Minimum Swaps to Move Zeros to End
difficulty: easy
tags: array, two-pointers
signature: int minimumSwaps(int[] nums)
---
Call the array `nums` **settled** when no `0` in it has a non-zero value
anywhere to its right. In a settled array the zeros form one block at the
back, and the other values fill every position in front of that block.

A **swap** picks two different positions of `nums`, anywhere in the array
(they need not be neighbours), and exchanges the values they hold. Return
the smallest number of swaps after which `nums` is settled.

Only the places of the zeros matter. The non-zero values may finish in any
order among themselves; they don't have to keep the order they started in.
An array that is settled from the start, such as one with no zeros or one
with nothing but zeros, needs `0` swaps.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `0 <= nums[i] <= 100`
