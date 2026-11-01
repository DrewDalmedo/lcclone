id: 219
slug: contains-duplicate-ii
title: Contains Duplicate II
difficulty: easy
tags: array, hash-table, sliding-window
signature: bool containsNearbyDuplicate(int[] nums, int k)
---
Positions in `nums` are numbered from `0`. Call two positions `i < j` a
**near repeat** when they hold the same value and lie at most `k` apart:
`nums[i] == nums[j]` and `j - i <= k`.

Return `true` if `nums` has at least one near repeat, and `false` if it has
none.

A position is never paired with itself, so with `k = 0` the answer is always
`false`. Only equal values pair up: `8` and `-8` are different values.

@examples

## Constraints

- `1 <= nums.length <= 10^5`
- `-10^9 <= nums[i] <= 10^9`
- `0 <= k <= 10^5`

## Follow-up

Checking the next `k` positions after each one takes `O(n * k)` time. Can
you answer in `O(n)` expected time?
