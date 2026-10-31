id: 191
slug: number-of-1-bits
title: Number of 1 Bits
difficulty: easy
tags: bit-manipulation, divide-and-conquer
signature: int hammingWeight(int n)
---
Write the positive integer `n` in base 2, without leading zeros, and return
how many of its binary digits are `1`: its population count, or Hamming
weight.

The examples give `n` in decimal; their explanations spell out the bits.

@examples

## Constraints

- `1 <= n <= 2^31 - 1`

## Follow-up

Looking at the bits one by one takes 31 steps. If the function had to run
on a huge number of inputs, could you make each call cheaper: say with a
small table of precomputed counts, by jumping straight from one `1` bit to
the next, or by adding up groups of neighbouring bits in parallel?
