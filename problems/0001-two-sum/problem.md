id: 1
slug: two-sum
title: Two Sum
difficulty: easy
tags: array, hash-table
signature: int[] twoSum(int[] nums, int target)
compare: unordered 0
---
You have a list of integers `nums` and a goal value `target`. Pick out two
entries of `nums` at **different indices** whose sum is exactly `target`, and
return those two indices as an array.

Every test is built so that exactly one such pair exists. An index may not be
paired with itself, but two different indices may hold equal values. The two
indices can be returned *in either order*.

@examples

## Constraints

- `2 <= nums.length <= 10^4`
- `-10^9 <= nums[i] <= 10^9`
- `-10^9 <= target <= 10^9`
- Exactly one pair of indices has values that add up to `target`.

## Follow-up

Trying every pair takes `O(n^2)` time. Can you find the pair while looking at
each element of `nums` only once?
