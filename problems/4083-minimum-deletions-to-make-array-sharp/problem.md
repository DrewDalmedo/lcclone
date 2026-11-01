id: 4083
slug: minimum-deletions-to-make-array-sharp
title: Minimum Deletions to Make Array Sharp
difficulty: easy
tags:
signature: int minDeletions(int[] nums)
---
Call a list of integers **sharp** when every two neighbours `x` and `y`
in it have a bitwise XOR other than zero, `x ^ y != 0`. Two integers XOR to
`0` exactly when they are equal, so a sharp list is one in which no element
equals the element right after it. A list of fewer than two elements has
no neighbours, so it is sharp.

Choose some positions of `nums`, as many as you like or none at all, and
erase the elements there. What is left is read in the order it had in
`nums`, and elements that were apart can become neighbours: erasing the
`5` from `[8,5,8]` leaves `[8,8]`, which is not sharp. Return the fewest
deletions that leave a sharp list.

A sharp list may hold the same value many times, as long as no two copies
end up side by side.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
