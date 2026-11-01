id: 258
slug: add-digits
title: Add Digits
difficulty: easy
tags: math, number-theory, simulation
signature: int addDigits(int num)
---
A step replaces `num` with the sum of its decimal digits. Keep taking steps
for as long as `num` is `10` or more, and return the number you end up
with, which is between `0` and `9`.

When `num` is below `10` from the start, no step is taken: return `num`
itself.

@examples

## Constraints

- `0 <= num <= 2^31 - 1`

## Follow-up

The answer can be read off `num` with a short formula. Can you find it, so
that the function runs in `O(1)` time with no loop and never calls itself?
