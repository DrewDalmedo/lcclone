id: 136
slug: single-number
title: Single Number
difficulty: easy
tags: bit-manipulation, array
signature: int singleNumber(int[] nums)
---
The integer array `nums` is made of matched pairs plus one extra element.
A pair is two copies of the same value, at any two positions of the array
(not necessarily side by side), and no two pairs share a value. The extra
element's value differs from the value of every pair, so it occurs in
`nums` only once.

Return the value of the extra element.

@examples

## Constraints

- `1 <= nums.length <= 3 * 10^4`
- `-3 * 10^4 <= nums[i] <= 3 * 10^4`
- Exactly one value occurs once in `nums`; each of the others occurs
  exactly twice.

## Follow-up

Counting with a table, or sorting first, is easy. Can you find the value in
`O(n)` time, using only `O(1)` extra memory?
