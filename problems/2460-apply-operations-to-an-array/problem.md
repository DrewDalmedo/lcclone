id: 2460
slug: apply-operations-to-an-array
title: Apply Operations to an Array
difficulty: easy
tags: array, two-pointers, simulation
signature: int[] applyOperations(int[] nums)
---
`nums` holds `n` non-negative integers. Transform it in two stages and
return the array you end up with.

1. **Merge.** Go along the pairs of neighbouring positions one at a time,
   from the left: positions `0` and `1`, then `1` and `2`, and so on up to
   `n - 2` and `n - 1`. Each time, look at the two values the positions
   hold *right then*, after the steps before. If the two are equal, the
   left one is doubled and the right one becomes `0`. If they differ,
   nothing happens.
2. **Pack.** Bring every non-zero value forward, without changing their
   order among themselves, and put the zeros behind them, so the array
   keeps its length `n`.

A value that has just been doubled is never looked at again in the first
stage, so merges don't chain. Zeros only move in the second stage, so a
`0` sitting between two equal values keeps them from merging, and values
that end up side by side after packing are not merged.

@examples

## Constraints

- `2 <= nums.length <= 2000`
- `0 <= nums[i] <= 1000`
