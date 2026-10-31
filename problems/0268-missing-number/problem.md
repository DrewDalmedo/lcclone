id: 268
slug: missing-number
title: Missing Number
difficulty: easy
tags: bit-manipulation, array, hash-table, math, binary-search, sorting
signature: int missingNumber(int[] nums)
---
Let `n` be the length of `nums`. The `n + 1` whole numbers from `0` to `n`
(both included) were meant to go into `nums`, but one of them was left out:
`nums` holds each of the other `n` numbers exactly once, in no particular
order.

Return the number that was left out. It can be any of `0` to `n`, the two
ends included.

@examples

## Constraints

- `1 <= nums.length <= 10^4`
- `0 <= nums[i] <= nums.length`
- The elements of `nums` are all different.

## Follow-up

Can you find it in `O(n)` time, using only `O(1)` extra memory?
