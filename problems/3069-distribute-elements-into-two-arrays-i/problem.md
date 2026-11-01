id: 3069
slug: distribute-elements-into-two-arrays-i
title: Distribute Elements Into Two Arrays I
difficulty: easy
tags: array, two-pointers, simulation
signature: int[] resultArray(int[] nums)
---
A board has two rows, the **top** row and the **bottom** row, both empty at
the start. The numbers of `nums` are written onto the board one at a time,
in the order they appear, each at the right end of one of the rows:

- the first number goes into the top row, and the second into the bottom
  row;
- each number after that goes into the top row if the rightmost number of
  the top row is greater than the rightmost number of the bottom row, and
  into the bottom row otherwise.

The number being placed takes no part in the choice: only the two rows'
rightmost numbers at that moment are compared, and each placement changes
the rightmost number of its row for the steps that follow.

When every number is on the board, return the top row read from left to
right followed by the bottom row read from left to right. The numbers in
`nums` are all different, so the two rightmost numbers are never equal.

@examples

## Constraints

- `3 <= nums.length <= 50`
- `1 <= nums[i] <= 100`
- The numbers in `nums` are all different.
