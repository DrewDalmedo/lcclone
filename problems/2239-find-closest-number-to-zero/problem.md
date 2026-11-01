id: 2239
slug: find-closest-number-to-zero
title: Find Closest Number to Zero
difficulty: easy
tags: array
signature: int findClosestNumber(int[] nums)
---
Mark every number of `nums` on a number line. Return the number whose mark
lies nearest to `0`, that is, the one with the smallest absolute value.

Two marks can be equally near only when they are the same number, or when
they are a positive number `d` and its negative `-d`. In the second case,
return `d`, the positive one, wherever the two stand in the array.

The answer is a value taken from `nums`, not its index.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `-10^5 <= nums[i] <= 10^5`
