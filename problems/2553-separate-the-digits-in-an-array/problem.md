id: 2553
slug: separate-the-digits-in-an-array
title: Separate the Digits in an Array
difficulty: easy
tags: array, simulation
signature: int[] separateDigits(int[] nums)
---
Picture a strip of squared paper. Starting at its left end, write the
numbers of `nums` on it one after another, in the order they come in
`nums`, one digit to a square. Each number is written the usual way, in
decimal starting from its leading digit, with no zeros in front. The next
number starts in the very next square, with no gap or separator.

Return the strip as an array: one element per square, from left to right,
each element a single digit from `0` to `9`.

A zero inside a number or at its end takes a square like any other digit,
so `70` fills two squares, `7` and `0`. The answer has as many elements as
all the numbers of `nums` have digits together.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `1 <= nums[i] <= 10^5`
