id: 2855
slug: minimum-right-shifts-to-sort-the-array
title: Minimum Right Shifts to Sort the Array
difficulty: easy
tags: array
signature: int minimumRightShifts(int[] nums)
---
`nums` holds `n` positive integers, no two of them equal. A **right shift**
moves every value one place to the right, and the value pushed off the
right-hand end comes back in at the left-hand end. One right shift turns
`[4,8,1]` into `[1,4,8]`, and another turns that into `[8,1,4]`.

Return the fewest right shifts after which the values of `nums` read in
increasing order from left to right. That number is `0` when `nums` is
already in increasing order. If no number of right shifts ever puts it in
increasing order, return `-1`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
- The values in `nums` are distinct.
