id: 3950
slug: exactly-one-consecutive-set-bits-pair
title: Exactly One Consecutive Set Bits Pair
difficulty: easy
tags: bit-manipulation
signature: bool consecutiveSetBits(int n)
---
Think of the binary form of `n` as a row of lamps, one per bit: a `1` bit
is a lit lamp and a `0` bit a dark one. Two lamps side by side, bit `i` and
bit `i + 1` for some `i >= 0`, form a **glowing pair** when both are lit.

Return `true` if the row has exactly one glowing pair, and `false` if it
has none or two or more.

Pairs may share a lamp, and each is counted. Three lit lamps in a row,
`111`, hold two glowing pairs (the lower two lamps and the upper two), and
in general a run of `r` lit lamps in a row holds `r - 1` of them. The zeros
above the highest `1` are dark lamps, so they never take part in a pair.

The examples give `n` in decimal; their explanations write it in binary.

@examples

## Constraints

- `0 <= n <= 10^5`
