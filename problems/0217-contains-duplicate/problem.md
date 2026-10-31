id: 217
slug: contains-duplicate
title: Contains Duplicate
difficulty: easy
tags: array, hash-table, sorting
signature: bool containsDuplicate(int[] nums)
---
Look for a repeat in the integer list `nums`: two different positions that
hold the same value.

Return `true` if `nums` has a repeat, and `false` if each value in it occurs
at only one position.

Only equal values make a repeat: `8` and `-8` are two different values.

@examples

## Constraints

- `1 <= nums.length <= 10^5`
- `-10^9 <= nums[i] <= 10^9`

## Follow-up

Comparing every pair of positions takes `O(n^2)` time. Can you answer in
`O(n log n)` time, or in `O(n)` expected time?
