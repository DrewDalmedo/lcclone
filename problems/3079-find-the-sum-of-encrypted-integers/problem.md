id: 3079
slug: find-the-sum-of-encrypted-integers
title: Find the Sum of Encrypted Integers
difficulty: easy
tags: array, math
signature: int sumOfEncryptedInt(int[] nums)
---
Take the decimal digits of a positive integer (without leading zeros) and
let `d` be the highest of them. **Encrypting** the integer gives the number
made of `d` repeated once per digit of the original, zeros included, so the
two have the same length. For instance, `4071` becomes `7777`, `60` becomes
`66` and `8` stays `8`.

Encrypt every number of `nums` separately and return the sum of the
encrypted numbers. A number that occurs several times in `nums` is
encrypted and added each time.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 1000`
