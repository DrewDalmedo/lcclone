id: 961
slug: n-repeated-element-in-size-2n-array
title: N-Repeated Element in Size 2N Array
difficulty: easy
tags: array, hash-table
signature: int repeatedNTimes(int[] nums)
---
The array `nums` has `2n` entries, for some `n >= 2`. They are one value
written `n` times, together with `n` other values that are written once
each: these are all different from one another and from the repeated
value. The entries come in any order, so the copies of the repeated value
can be anywhere.

Return the repeated value.

@examples

## Constraints

- `4 <= nums.length <= 10^4`
- `nums.length` is even.
- `0 <= nums[i] <= 10^4`
- One value occurs exactly `nums.length / 2` times in `nums`, and every
  other value occurs at most once.
