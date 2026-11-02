id: 3750
slug: minimum-number-of-flips-to-reverse-binary-string
title: Minimum Number of Flips to Reverse Binary String
difficulty: easy
tags: bit-manipulation, math, two-pointers, string
signature: int minimumFlips(int n)
---
Take the binary digits of the positive integer `n`, from its highest `1`
bit down to its lowest bit, as a string `s`. The **mirror** of `s` is `s`
read from right to left: a string of the same length, which starts with `0`
whenever `n` is even.

Flipping a bit of `s` replaces a `0` there with `1`, or a `1` with `0`.
Return the fewest bits you must flip so that `s` becomes its mirror.

The mirror is taken once, from `n`'s binary form before any flip, and stays
fixed while `s` changes. Both strings keep every character, leading zeros
included, and are compared position by position.

`n` is shown in decimal in the examples, with its bit string `s` worked out
in each explanation.

@examples

## Constraints

- `1 <= n <= 10^9`
