id: 2544
slug: alternating-digit-sum
title: Alternating Digit Sum
difficulty: easy
tags: math
signature: int alternateDigitSum(int n)
---
Two players share out the decimal digits of `n` by taking turns. Reading
`n` from left to right, the first player takes the leading digit, the
second player takes the digit after it, the first player the one after
that, and so on until every digit is taken. So the first player ends up
with the 1st, 3rd, 5th, ... digits and the second player with the 2nd, 4th,
6th, ... ones. Each player scores the sum of the digits they took; a `0`
scores nothing, but it still uses up a turn.

Return the first player's score minus the second player's score. The
result may be `0` or negative.

@examples

## Constraints

- `1 <= n <= 10^9`
