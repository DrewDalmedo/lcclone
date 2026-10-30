id: 190
slug: reverse-bits
title: Reverse Bits
difficulty: easy
tags: divide-and-conquer, bit-manipulation
signature: uint32 reverseBits(uint32 n)
---
`n` is a 32-bit unsigned integer. Write it as exactly 32 bits, with leading
zeros, and reverse their order: the lowest bit becomes the highest, the
second lowest the second highest, and so on. Return the number those
reversed bits make.

The examples write numbers in decimal, so their bits are spelled out in the
explanations.

@examples

## Constraints

- `0 <= n <= 2^32 - 1`

## Follow-up

If this were called millions of times, could you make it faster by reversing
whole groups of bits at once?
