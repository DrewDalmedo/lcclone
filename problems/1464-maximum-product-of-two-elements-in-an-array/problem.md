id: 1464
slug: maximum-product-of-two-elements-in-an-array
title: Maximum Product of Two Elements in an Array
difficulty: easy
tags: array, sorting, heap-priority-queue
signature: int maxProduct(int[] nums)
---
Knock one off each number before multiplying: for two indices `i` and `j`
of `nums` with `i != j`, the **score** of the pair is
`(nums[i] - 1) * (nums[j] - 1)`. Return the highest score that any such
pair reaches.

A single position can't supply both factors. Two positions that hold the
same number are fine, though, so a value that appears twice in `nums` can
be paired with itself. Every number is at least `1`, so no score is
negative.

@examples

## Constraints

- `2 <= nums.length <= 500`
- `1 <= nums[i] <= 1000`
