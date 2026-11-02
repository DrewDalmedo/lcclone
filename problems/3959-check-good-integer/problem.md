id: 3959
slug: check-good-integer
title: Check Good Integer
difficulty: easy
tags: math, simulation
signature: bool checkGoodInteger(int n)
---
Two totals are taken over the decimal digits of `n`: `D` adds up the
digits themselves, and `Q` adds up their squares. For `n = 2608`, that
gives `D = 2 + 6 + 0 + 8 = 16` and `Q = 4 + 36 + 0 + 64 = 104`.

`n` is **good** when `Q` exceeds `D` by `50` or more, that is, when
`Q - D >= 50`; a gap of exactly `50` is enough. The function answers
`true` for a good `n` and `false` for any other.

A digit that occurs several times is counted each time. Zeros are digits
like the others, though they add nothing to either total.

@examples

## Constraints

- `1 <= n <= 10^9`
