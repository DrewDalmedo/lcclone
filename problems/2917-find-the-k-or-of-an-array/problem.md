id: 2917
slug: find-the-k-or-of-an-array
title: Find the K-or of an Array
difficulty: easy
tags: bit-manipulation, array
signature: int findKOr(int[] nums, int k)
---
Treat each bit position as a vote. Bit `b` stands for the value `2^b`, so
bit `0` is the ones place, bit `1` the twos place, and so on. Every
element of `nums` that has a `1` at position `b` casts one vote for that
bit; each element votes on its own, so a value that appears twice casts
two votes.

A bit **wins** when it gets at least `k` votes. Return the number whose
binary form has a `1` at every winning position and a `0` everywhere
else, or `0` if no bit wins.

This number is called the **K-or** of `nums`: with `k = 1` it is the
bitwise OR of all the elements, and with `k = nums.length` their bitwise
AND.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `0 <= nums[i] < 2^31`
- `1 <= k <= nums.length`
