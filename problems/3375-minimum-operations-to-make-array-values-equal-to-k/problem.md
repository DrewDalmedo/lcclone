id: 3375
slug: minimum-operations-to-make-array-values-equal-to-k
title: Minimum Operations to Make Array Values Equal to K
difficulty: easy
tags: array, hash-table
signature: int minOperations(int[] nums, int k)
---
`nums[i]` is the height of tower `i`. You may **trim** the towers any
number of times. Each trim uses a whole number `h` of your choice and cuts
every tower taller than `h` down to height exactly `h`, leaving the other
towers alone. There is one restriction on `h`: just before the cut, the
towers taller than `h` must all have the same height.

`h` does not have to be one of the heights. If no tower is taller than
`h`, the restriction holds and the trim changes nothing. Trims never make
a tower taller.

Return the fewest trims after which every tower has height exactly `k`, or
`-1` if no sequence of trims can get there.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
- `1 <= k <= 100`
