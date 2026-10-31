id: 169
slug: majority-element
title: Majority Element
difficulty: easy
tags: array, hash-table, divide-and-conquer, counting, sorting
signature: int majorityElement(int[] nums)
---
Let `n` be the length of `nums`. A value is the **majority** of `nums` when
it fills more than half of the positions, that is, when it appears at least
`floor(n / 2) + 1` times. In an array of 9 elements that takes 5 copies or
more, and in an array of 10 elements 6 copies or more.

`nums` always has a majority value. Return it. (There can't be two: together
they would fill more than `n` positions.)

@examples

## Constraints

- `1 <= nums.length <= 5 * 10^4`
- `-10^9 <= nums[i] <= 10^9`
- Some value fills more than half of the positions of `nums`.

## Follow-up

Can you find the majority in `O(n)` time, using only `O(1)` extra memory?
