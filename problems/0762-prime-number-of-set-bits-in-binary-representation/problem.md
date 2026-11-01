id: 762
slug: prime-number-of-set-bits-in-binary-representation
title: Prime Number of Set Bits in Binary Representation
difficulty: easy
tags: bit-manipulation, math
signature: int countPrimeSetBits(int left, int right)
---
A raffle has one ticket for each integer `x` with `left <= x <= right`.
Ticket `x` **wins** when `x`, written in binary, has a prime number of `1`
digits. Return how many tickets win.

It is the number of `1`s that has to be prime, not `x` itself: ticket `20`
(`10100`, two `1`s) wins although 20 is not prime, and ticket `23`
(`10111`, four `1`s) loses although 23 is. A prime is a whole number of at least `2` whose only
positive divisors are `1` and itself. So `2` is prime and `1` is not: a
power of two, with its single `1`, never wins.

@examples

## Constraints

- `1 <= left <= right <= 10^6`
- `0 <= right - left <= 10^4`
