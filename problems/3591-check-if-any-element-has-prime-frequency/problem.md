id: 3591
slug: check-if-any-element-has-prime-frequency
title: Check if Any Element Has Prime Frequency
difficulty: easy
tags: array, hash-table, math, counting, number-theory
signature: bool checkPrimeFrequency(int[] nums)
---
Split the entries of `nums` into piles, one pile for each distinct value, so
that equal entries end up together and different ones apart. The **size**
of a pile is the number of entries in it.

Return `true` if the size of at least one pile is a prime number, and
`false` if no pile has a prime size.

A whole number `p` is prime when `p >= 2` and none of `2, 3, ..., p - 1`
divides it evenly; the smallest primes are `2`, `3`, `5`, `7` and `11`.
A size of `1` is not prime, so a value that occurs only once never makes
the answer `true`. Only the pile sizes are tested: whether the values
themselves, the number of piles or the length of `nums` are prime makes no
difference.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `0 <= nums[i] <= 100`
