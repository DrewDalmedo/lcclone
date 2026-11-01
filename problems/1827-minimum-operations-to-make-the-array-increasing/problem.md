id: 1827
slug: minimum-operations-to-make-the-array-increasing
title: Minimum Operations to Make the Array Increasing
difficulty: easy
tags: greedy, array
signature: int minOperations(int[] nums)
---
A row of stacks of crates stands from left to right; `nums[i]` is the
number of crates in stack `i`. The row should become a staircase that only
climbs: every stack must hold **more** crates than the stack to its left
(the same number is not enough).

The only move allowed is to put one new crate on top of any stack you
choose. Crates are never taken away, and the stacks never change places.

Return the smallest number of moves that turns the row into such a
staircase. A row of one stack already is a staircase, and so is a row that
climbs already; both need `0` moves.

@examples

## Constraints

- `1 <= nums.length <= 5000`
- `1 <= nums[i] <= 10^4`
