id: 3701
slug: compute-alternating-sum
title: Compute Alternating Sum
difficulty: easy
tags: array, simulation
signature: int alternatingSum(int[] nums)
---
A marker starts at position `0` on a number line. It makes one move for
each element of `nums`, taking the elements from left to right, and the
moves alternate in direction. The first move carries it `nums[0]` units
toward the larger numbers, the second `nums[1]` units back toward the
smaller numbers, the third `nums[2]` units toward the larger numbers again,
and so on.

So an element at an even index (`0`, `2`, `4`, ...) moves the marker
forward and an element at an odd index (`1`, `3`, `5`, ...) moves it back.
The final position is `nums[0] - nums[1] + nums[2] - nums[3] + ...`, with
the signs alternating up to the last element.

Return the marker's position after its last move. It may be `0` or
negative.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
