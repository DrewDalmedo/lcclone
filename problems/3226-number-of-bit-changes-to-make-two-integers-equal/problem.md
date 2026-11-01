id: 3226
slug: number-of-bit-changes-to-make-two-integers-equal
title: Number of Bit Changes to Make Two Integers Equal
difficulty: easy
tags: bit-manipulation
signature: int minChanges(int n, int k)
---
A positive integer is a sum of distinct powers of two in exactly one way,
one power for each `1` in its binary form: `22 = 16 + 4 + 2`. Call those
powers the integer's **parts**.

A move takes one part away from `n`: it subtracts `2^i` from `n`, where
`2^i` is one of the parts `n` has at that moment. Nothing is ever added, so
a power of two that is not a part of `n` can never become one. Moves change
only `n`; `k` stays as it is.

Return how many moves it takes to get from `n` to exactly `k`, or `-1`
when `k` is out of reach. When the two start out equal, no moves are
needed and the answer is `0`.

Each example's note writes both numbers as sums of their parts.

@examples

## Constraints

- `1 <= n, k <= 10^6`
