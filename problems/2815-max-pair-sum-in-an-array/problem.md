id: 2815
slug: max-pair-sum-in-an-array
title: Max Pair Sum in an Array
difficulty: easy
tags: array, hash-table
signature: int maxSum(int[] nums)
---
Call the **top digit** of a positive integer the biggest of the digits it
is written with in decimal. The top digit of `4091` is `9`, that of `505`
is `5`, that of `10000` is `1`, and a one-digit number is its own top
digit.

Find two entries of `nums`, at different positions, that have the same
top digit, and make their sum as large as possible. Return that sum, or
`-1` if the top digit of every entry differs from those of all the others.

The two entries may be equal in value, as long as they sit at different
positions.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `1 <= nums[i] <= 10^4`
