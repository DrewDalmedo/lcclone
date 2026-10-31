id: 283
slug: move-zeroes
title: Move Zeroes
difficulty: easy
tags: array, two-pointers
signature: void moveZeroes(int[] nums)
check: param nums
---
Rearrange `nums` **in place** so that every zero ends up at the back and
every other value at the front. The non-zero values must stay in the same
order relative to each other as they were before.

The function returns nothing: `nums` itself is checked after the call. It
keeps its length and its values; only their positions change. The examples
show `nums` as it must be afterwards.

@examples

## Constraints

- `1 <= nums.length <= 10^4`
- `-2^31 <= nums[i] <= 2^31 - 1`

## Follow-up

Can you do it without a second array, with `O(1)` extra memory? And with as
few writes to `nums` as possible?
