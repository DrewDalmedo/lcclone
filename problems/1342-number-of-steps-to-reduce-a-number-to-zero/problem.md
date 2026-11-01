id: 1342
slug: number-of-steps-to-reduce-a-number-to-zero
title: Number of Steps to Reduce a Number to Zero
difficulty: easy
tags: bit-manipulation, math
signature: int numberOfSteps(int num)
---
A counter shows the number `num` and is worn down to `0` one **step** at a
time. Each step looks at what the counter shows: an odd number loses `1`,
and an even number is cut in half. The counter stops the moment it shows
`0`, so if it starts at `0` it takes no steps at all.

Return how many steps the counter takes to get from `num` to `0`.

@examples

## Constraints

- `0 <= num <= 10^6`
