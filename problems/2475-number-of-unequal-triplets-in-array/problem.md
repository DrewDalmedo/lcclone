id: 2475
slug: number-of-unequal-triplets-in-array
title: Number of Unequal Triplets in Array
difficulty: easy
tags: array, hash-table, sorting
signature: int unequalTriplets(int[] nums)
---
A string of beads lies on a table, and `nums[i]` is the colour of the bead
at position `i`, given as a number. You want to pick three of the beads,
at three different positions, so that all three colours differ: no two of
the picked beads have the same colour. Return the number of ways to do it.

A way is a set of three positions. Picking the same three positions in
another order is the same way, counted once. Beads of one colour at
different positions are still different beads, so the same three colours,
taken from other positions, make another way.

@examples

## Constraints

- `3 <= nums.length <= 100`
- `1 <= nums[i] <= 1000`
