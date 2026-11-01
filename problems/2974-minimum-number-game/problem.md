id: 2974
slug: minimum-number-game
title: Minimum Number Game
difficulty: easy
tags: array, sorting, simulation, heap-priority-queue
signature: int[] numberGame(int[] nums)
---
The game builds a new array `arr` out of `nums`, an array of even length,
round by round. Each round takes the two smallest values still left in
`nums` out of it, and appends both to the end of `arr`: first the larger
of the two, then the smaller. The rounds go on until `nums` is empty, and
the answer is `arr`.

A round always takes out exactly two elements. If the smallest value left
occurs several times, a round may take two copies of it, and any copies it
doesn't take stay for later rounds. The order of the elements in `nums`
doesn't affect the answer; only their values do.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `nums.length` is even.
- `1 <= nums[i] <= 100`
