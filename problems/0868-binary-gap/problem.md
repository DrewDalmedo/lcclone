id: 868
slug: binary-gap
title: Binary Gap
difficulty: easy
tags: bit-manipulation
signature: int binaryGap(int n)
---
Write the positive integer `n` in binary and number its digits by
**place**, from the right: the last digit is at place `0`, the one before
it at place `1`, and so on.

List the places that hold a `1`, from lowest to highest. Two places next to
each other in that list hold **neighbouring** 1s, with no other `1` between
them. Their distance is the higher place minus the lower, so two 1s side by
side are at distance `1`. Zeros below the lowest `1` don't count.

Return the largest distance between neighbouring 1s in `n`, or `0` if `n`
has a single `1`.

The examples give `n` in decimal; their explanations spell out the binary.

@examples

## Constraints

- `1 <= n <= 10^9`
