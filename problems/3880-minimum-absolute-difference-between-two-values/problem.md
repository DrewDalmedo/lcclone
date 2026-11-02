id: 3880
slug: minimum-absolute-difference-between-two-values
title: Minimum Absolute Difference Between Two Values
difficulty: easy
tags: array, enumeration
signature: int minAbsoluteDifference(int[] nums)
---
Every entry of `nums` is `0`, `1` or `2`. Find a `1` and a `2` in `nums`
that sit as close together as possible, and return how far apart they
are. The distance is measured between positions, not values: if
`nums[i] == 1` and `nums[j] == 2`, the two are `|i - j|` apart. The `2`
may come before the `1` or after it, and every `0` between them still
counts towards the distance.

If `nums` contains no `1`, or no `2`, there is nothing to measure: return
`-1`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `nums[i]` is `0`, `1` or `2`.
