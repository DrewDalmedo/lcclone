id: 1636
slug: sort-array-by-increasing-frequency
title: Sort Array by Increasing Frequency
difficulty: easy
tags: array, hash-table, sorting
signature: int[] frequencySort(int[] nums)
---
Count how often each value occurs in `nums`, and call that number the
value's **count**. Rearrange `nums` so that rarer values come first, and
return the result. Two different values are placed by these rules:

- the one with the smaller count goes first;
- if their counts are equal, the larger one goes first. Values compare as
  signed integers, so `0` goes before `-2`, and `-2` before `-9`.

The result is a reordering of `nums`: no value is added or removed. All
copies of a value have the same count, so they sit together in one block,
and the rules leave only one possible order.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `-100 <= nums[i] <= 100`
