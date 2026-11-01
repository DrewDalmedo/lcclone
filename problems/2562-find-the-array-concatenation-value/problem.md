id: 2562
slug: find-the-array-concatenation-value
title: Find the Array Concatenation Value
difficulty: easy
tags: array, two-pointers, simulation
signature: long findTheArrayConcVal(int[] nums)
---
The **concatenation** of two positive integers `a` and `b` is the number
whose decimal digits are the digits of `a` followed by the digits of `b`:
`31` and `7` concatenate to `317`, while `7` and `31` concatenate to
`731`.

Start with a score of `0` and repeat until `nums` is empty:

- if `nums` has two or more elements, remove its first element `a` and its
  last element `b`, and add the concatenation of `a` and `b` (in that
  order) to the score;
- if a single element is left, remove it and add it to the score as it
  is, with nothing concatenated.

Return the final score.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `1 <= nums[i] <= 10^4`
