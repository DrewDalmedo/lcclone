id: 4038
slug: count-integers-appearing-in-a-single-block
title: Count Integers Appearing in a Single Block
difficulty: easy
tags: array, hash-table, counting
signature: int countSpecialIntegers(int[] nums)
---
Pick a value `x` that occurs in `nums` and mark the indices where it
occurs. If the marked indices form one gap-free stretch `i, i + 1, ...,
j`, then `x` appears in a **single block**: there is one contiguous piece
of the array that holds every copy of `x` and nothing but `x`. If instead
some other value turns up between two copies of `x`, then `x` is split into
two or more blocks.

A value that occurs exactly once forms a block of length `1` on its own, so
it appears in a single block too.

Return the number of distinct values of `nums` that appear in a single
block. The length of the block makes no difference: a value with many
copies side by side adds `1` to the answer, the same as a value that occurs
once.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
