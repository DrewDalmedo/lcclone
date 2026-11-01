id: 1389
slug: create-target-array-in-the-given-order
title: Create Target Array in the Given Order
difficulty: easy
tags: array, simulation
signature: int[] createTargetArray(int[] nums, int[] index)
---
Build a row of numbers, starting from an empty row, by slotting in the
numbers of `nums` one at a time: first `nums[0]`, then `nums[1]`, and so
on. When it is the turn of `nums[i]`, slot it in so that exactly
`index[i]` numbers stand to its left; the number that stood at that spot
and every number to its right each slide one place to the right to make
room. If `index[i]` equals the number of numbers already in the row, the
new number simply goes at the right end.

Return the row, from left to right, once every number of `nums` has been
slotted in.

Positions count from `0`. When `nums[i]` takes its turn, the row holds
`i` numbers, so `index[i]`, which is at most `i` (see the constraints),
always names a possible spot.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `nums.length == index.length`
- `0 <= nums[i] <= 100`
- `0 <= index[i] <= i`
