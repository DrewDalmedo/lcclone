id: 1480
slug: running-sum-of-1d-array
title: Running Sum of 1d Array
difficulty: easy
tags: array, prefix-sum
signature: int[] runningSum(int[] nums)
---
A marker sits at `0` on a number line, and `nums` is a list of moves made
in order. Move `i` shifts the marker by `nums[i]`: that many places to the
right when it is positive, to the left when it is negative, and nowhere
when it is `0`. The marker may pass `0` and stand on negative numbers.

Return where the marker stands after each move: an array as long as `nums`
whose entry `i` is `nums[0] + nums[1] + ... + nums[i]`, the sum of moves `0`
to `i`, move `i` itself included. So entry `0` is `nums[0]`; the starting
point `0` is not part of the answer.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `-10^6 <= nums[i] <= 10^6`
