id: 3314
slug: construct-the-minimum-bitwise-array-i
title: Construct the Minimum Bitwise Array I
difficulty: easy
tags: bit-manipulation, array
signature: int[] minBitwiseArray(int[] nums)
---
Every number in `nums` is prime. For each prime `p` of the list, look for
the smallest integer `a >= 0` such that

`a | (a + 1) == p`

where `|` is the bitwise OR: a bit of the result is 1 exactly when that bit
is 1 in `a`, in `a + 1`, or in both. Several values of `a` may satisfy the
equation; only the smallest one is wanted. If none does, the answer for `p`
is `-1`.

Return the answers as an array with one entry per entry of `nums`, in the
same order. Each entry is worked out from its own prime alone, so equal
primes get equal answers.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `2 <= nums[i] <= 1000`
- Every `nums[i]` is a prime number.
