id: 2656
slug: maximum-sum-with-exactly-k-elements
title: Maximum Sum With Exactly K Elements
difficulty: easy
tags: greedy, array
signature: int maximizeSum(int[] nums, int k)
---
Think of each element of `nums` as a counter showing a number. You have
exactly `k` moves to make. In a move you pick one counter: you earn the
number it shows at that moment, and then the counter goes up by `1`, so
that it shows one more for every later move.

Nothing stops you from picking the same counter in several moves, or in all
of them, and `k` may be larger than the number of counters. Return the most
you can earn over the `k` moves put together.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
- `1 <= k <= 100`
