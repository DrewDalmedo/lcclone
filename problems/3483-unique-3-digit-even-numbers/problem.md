id: 3483
slug: unique-3-digit-even-numbers
title: Unique 3-Digit Even Numbers
difficulty: easy
tags: recursion, array, hash-table, enumeration
signature: int totalNumbers(int[] digits)
---
The array `digits` is a pool of decimal digits, in no particular order, and
a digit may occur in it several times. An integer `x` **fits** the pool when
no digit occurs more often in the decimal form of `x` than it does in
`digits`. With the pool `[3,0,3,8]`, for instance, `338` and `830` fit, but
`808` doesn't: it has two `8`s and the pool only one.

Return how many integers from `100` to `999` are even and fit the pool.

An integer from `100` to `999` has three digits and doesn't start with
`0`. So each number counted is made of three entries of the pool, each
entry used at most once, and a `0` can only be its middle or last digit.
An integer that several selections of entries can build still adds only
`1` to the count.

@examples

## Constraints

- `3 <= digits.length <= 10`
- `0 <= digits[i] <= 9`
