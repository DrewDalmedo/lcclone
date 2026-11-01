id: 2220
slug: minimum-bit-flips-to-convert-number
title: Minimum Bit Flips to Convert Number
difficulty: easy
tags: bit-manipulation
signature: int minBitFlips(int start, int goal)
---
A control panel has a long row of toggle switches, and shows a number in
binary with them: switch `k` stands for the value `2^k`, and is up when the
number has that binary digit set and down when it doesn't. The number is
the sum of the values of the switches that are up.

A move flips one switch of your choice, from down to up or from up to down.
Every switch can be flipped, including the ones above the highest switch
that is up.

The panel shows `start`. Return the fewest moves that make it show `goal`.

Each example lists `start` and `goal` as ordinary decimal numbers, and its
note shows the switches as binary digits, highest switch on the left.

@examples

## Constraints

- `0 <= start <= 10^9`
- `0 <= goal <= 10^9`
