id: 2269
slug: find-the-k-beauty-of-a-number
title: Find the K-Beauty of a Number
difficulty: easy
tags: math, string, sliding-window
signature: int divisorSubstrings(int num, int k)
---
Write the positive integer `num` in decimal, with no leading zeros, and lay
a frame exactly `k` digits wide over it. Start with the frame over the
first `k` digits, then move it right one digit at a time until it covers
the last digit. At each position, read the `k` digits inside the frame as a
number; zeros at the front of the frame just fall away, so the digits `0`
and `6` read as `6`, and a frame of zeros reads as `0`.

The **k-beauty** of `num` is the number of frame positions whose number
divides `num` exactly, with no remainder. A position that reads `0` never
counts, since no number is divisible by zero. Positions are counted one by
one: if two of them show the same digits, each counts on its own.

Return the k-beauty of `num`.

@examples

## Constraints

- `1 <= num <= 10^9`
- `1 <= k <= d`, where `d` is the number of digits of `num`
