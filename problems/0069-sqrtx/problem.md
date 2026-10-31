id: 69
slug: sqrtx
title: Sqrt(x)
difficulty: easy
tags: math, binary-search
signature: int mySqrt(int x)
---
Return the largest integer `r` whose square `r * r` is at most `x`. Put
another way: take the square root of `x` and drop everything after the
decimal point.

Work it out yourself: calling `sqrt`, `pow` or any other library function
for roots or powers defeats the purpose, even though the tests only check
the value you return.

@examples

## Constraints

- `0 <= x <= 2^31 - 1`

## Follow-up

Trying `r = 0, 1, 2, ...` in turn can take over 46,000 steps. Can you find
`r` in `O(log x)` steps?
