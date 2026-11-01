id: 1995
slug: count-special-quadruplets
title: Count Special Quadruplets
difficulty: easy
tags: array, hash-table, enumeration
signature: int countQuadruplets(int[] nums)
---
Pick four different indices of `nums` and read them from left to right.
The pick is a **special quadruplet** when the value at the last of the four
indices is exactly the sum of the values at the other three.

Return how many special quadruplets `nums` has.

- Only the rightmost of the four indices may hold the sum. The three values
  that add up to it may come in any order among themselves, but all three
  must lie to its left. So the order of `nums` matters: rearranging it can
  change the answer.
- Picks are compared by their indices, not by their values. Taking the same
  values from other indices is another pick, and it counts again.

@examples

## Constraints

- `4 <= nums.length <= 50`
- `1 <= nums[i] <= 100`
