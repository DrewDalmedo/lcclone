id: 414
slug: third-maximum-number
title: Third Maximum Number
difficulty: easy
tags: array, sorting
signature: int thirdMax(int[] nums)
---
Write down the values that occur in `nums`, each one once however many
times it occurs, and rank them from largest to smallest: the largest value
has rank 1, the next smaller value rank 2, and so on. Return the value with
rank 3.

When `nums` holds fewer than three different values there is no rank 3, and
you return the value with rank 1, the largest element of `nums`, instead.

@examples

## Constraints

- `1 <= nums.length <= 10^4`
- `-2^31 <= nums[i] <= 2^31 - 1`

## Follow-up

Sorting `nums` takes `O(n log n)` time. Can you find the answer in a single
pass, in `O(n)` time?
