id: 3423
slug: maximum-difference-between-adjacent-elements-in-a-circular-array
title: Maximum Difference Between Adjacent Elements in a Circular Array
difficulty: easy
tags: array
signature: int maxAdjacentDistance(int[] nums)
---
A running track is a closed loop with `n` checkpoints along it, numbered
`0` to `n - 1` in the order a runner reaches them. `nums[i]` is the height
of checkpoint `i`, measured from a reference level, so it can be negative.
After checkpoint `n - 1` the track leads back to checkpoint `0`.

Each stretch of the loop runs from checkpoint `i` to checkpoint
`(i + 1) mod n`, and along it the runner climbs or descends by
`|nums[i] - nums[(i + 1) mod n]|`, the distance between the two heights,
which is never negative. Return the largest climb or descent over the `n`
stretches. The last stretch, from checkpoint `n - 1` back to checkpoint
`0`, counts like any other; with only two checkpoints, both stretches join
the same pair.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `-100 <= nums[i] <= 100`
