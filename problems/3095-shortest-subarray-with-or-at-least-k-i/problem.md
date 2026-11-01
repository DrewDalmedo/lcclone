id: 3095
slug: shortest-subarray-with-or-at-least-k-i
title: Shortest Subarray With OR at Least K I
difficulty: easy
tags: bit-manipulation, array, sliding-window
signature: int minimumSubarrayLength(int[] nums, int k)
---
A **slice** of `nums` starts at some index `i`, ends at some index
`j >= i`, and takes every element from `nums[i]` through `nums[j]`. Join
its elements with the bitwise OR operator (`|`), and call the result the
slice's **OR value**.

Return the number of elements in the shortest slice whose OR value is
greater than or equal to `k`. If no slice reaches `k`, return `-1`.

The OR value and `k` are compared as plain numbers, not as sets of bits:
an OR value need not contain every bit of `k`, it only has to be at least
as large. When `k` is `0`, a single element is always enough, even a `0`.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `0 <= nums[i] <= 50`
- `0 <= k < 64`
