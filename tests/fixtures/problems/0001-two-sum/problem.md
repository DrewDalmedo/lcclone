# Fixture pack for the unit and CLI tests.
id: 1
slug: two-sum
title: Two Sum
difficulty: easy

# Unknown keys such as compare are kept for later milestones.
tags: array, hash-table
signature: int[] twoSum(int[] nums, int target)
compare: unordered 0
---
You are given an array of integers `nums` and a value `target`. Find **two
different positions** in `nums` whose values add up to `target`, and return
those positions.

Every input has exactly one such pair, and a position cannot be paired with
itself. Return the two positions *in either order*.

@examples

## Constraints

- `2 <= nums.length <= 5000`
- `-10^6 <= nums[i], target <= 10^6`
- Exactly one pair adds up to `target`.

## Follow-up

Checking every pair with two nested loops takes `O(n^2)` time. Can you find
the answer in a single pass over `nums`?
