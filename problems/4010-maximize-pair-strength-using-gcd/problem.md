id: 4010
slug: maximize-pair-strength-using-gcd
title: Maximize Pair Strength Using GCD
difficulty: easy
tags: array, math, enumeration, number-theory
signature: long maxPairStrength(int[] nums)
---
For two positive numbers `a` and `b`, let `g = gcd(a, b)` be their greatest
common divisor (the largest whole number that divides both), and call
`a * b / g^2` their **strength**. Since `g` divides each of them, the
strength is the whole number `(a / g) * (b / g)`.

Look at every way to pick two entries of `nums` at different indices, and
return the highest strength that any such pair reaches. An entry can't be
paired with itself, but two different indices holding equal values do form
a pair, of strength `1`.

@examples

## Constraints

- `2 <= nums.length <= 2000`
- `1 <= nums[i] <= 10^5`
