id: 1979
slug: find-greatest-common-divisor-of-array
title: Find Greatest Common Divisor of Array
difficulty: easy
tags: array, math, number-theory
signature: int findGCD(int[] nums)
---
Only two values of `nums` matter here: its smallest value, `lo`, and its
largest value, `hi`. Return their greatest common divisor, that is the
largest positive integer `d` such that both `lo` and `hi` are multiples
of `d`.

The values that lie between `lo` and `hi` play no part, even when they
share fewer divisors with `lo` and `hi` than these two share with each
other. When all the values of `nums` are equal, `lo` and `hi` are the same
number, and that number is the answer.

@examples

## Constraints

- `2 <= nums.length <= 1000`
- `1 <= nums[i] <= 1000`
