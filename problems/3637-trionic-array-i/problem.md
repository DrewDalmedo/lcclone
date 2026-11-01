id: 3637
slug: trionic-array-i
title: Trionic Array I
difficulty: easy
tags: array
signature: bool isTrionic(int[] nums)
---
Draw `nums` as a line chart: one point at height `nums[i]` for each index
`i`, and a segment joining each point to the next one. Call `nums`
**trionic** when its chart is shaped like the letter N, read from left to
right:

1. it rises,
2. then it falls,
3. then it rises again until the last point,

and each of those three parts has at least one segment. A segment between
two equal values is horizontal, and an N has no horizontal segments, so
two equal neighbours anywhere rule the array out. The chart may turn only
twice: one that goes on to fall again at the end is not an N.

In terms of indices: the chart's top corner is at some index `p` and its
bottom corner at some index `q`, with `0 < p < q < n - 1`. Every step
from index `0` to index `p` must go strictly up, every step from `p` to
`q` strictly down, and every step from `q` to the last index `n - 1`
strictly up again. Only the directions matter: the last part may climb
higher than `nums[p]`, and `nums[0]` may lie below `nums[q]`.

Return `true` if `nums` is trionic, and `false` otherwise.

@examples

## Constraints

- `n == nums.length`
- `3 <= n <= 100`
- `-1000 <= nums[i] <= 1000`
