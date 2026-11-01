id: 3158
slug: find-the-xor-of-numbers-which-appear-twice
title: Find the XOR of Numbers Which Appear Twice
difficulty: easy
tags: bit-manipulation, array, hash-table
signature: int duplicateNumbersXOR(int[] nums)
---
No value appears in `nums` more than twice. Call a value **doubled** when
it appears exactly twice; its two copies can stand anywhere in the array,
not necessarily next to each other.

Return the bitwise XOR of the doubled values, using each doubled value a
single time, not once per copy. Values that occur only once play no part.
If no value is doubled, the answer is `0`; if exactly one value is doubled,
the answer is that value.

The XOR of a list of numbers has a `1` in exactly those bit positions where
an odd number of them have a `1`. So it can be `0` even when some values
are doubled.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 50`
- No value appears in `nums` more than twice.
