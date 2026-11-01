id: 2566
slug: maximum-difference-by-remapping-a-digit
title: Maximum Difference by Remapping a Digit
difficulty: easy
tags: greedy, math
signature: int minMaxDifference(int num)
---
A **recoding** of the positive integer `num` picks two digits `x` and `y`,
each from `0` to `9`. Write `num` in decimal the usual way, with no zeros
in front, and turn every `x` in it into a `y`, all at once. The other
digits keep their places and values. `y` may equal `x`, and `x` need not
occur in `num`; either way nothing changes.

The rewritten digits are then read as a decimal number. Zeros at the front
are allowed and simply drop out: recoding `3530` with `x = 3` and `y = 0`
gives `0500`, which is `500`, and a result of all zeros is `0`.

Let `hi` be the largest number a single recoding of `num` can give, and
`lo` the smallest. They may come from two different recodings. Return
`hi - lo`.

@examples

## Constraints

- `1 <= num <= 10^8`
