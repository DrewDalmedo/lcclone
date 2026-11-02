id: 3908
slug: valid-digit-number
title: Valid Digit Number
difficulty: easy
tags: math
signature: bool validDigit(int n, int x)
---
Write `n` as an ordinary decimal numeral, with no leading zeros (zero is
the numeral `0`). Return `true` when both of these hold, and `false`
otherwise:

- the leftmost digit of the numeral is **not** `x`;
- the digit `x` appears at least once somewhere to the right of it.

So `x` has to be present, just never in front. Once the leftmost digit is
`x`, the answer is `false` however often `x` appears later, and a numeral
of a single digit can never pass.

@examples

## Constraints

- `0 <= n <= 10^5`
- `0 <= x <= 9`
