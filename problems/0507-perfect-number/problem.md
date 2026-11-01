id: 507
slug: perfect-number
title: Perfect Number
difficulty: easy
tags: math
signature: bool checkPerfectNumber(int num)
---
Add up every positive whole number that is smaller than `num` and divides
it exactly, leaving no remainder. Return `true` if the total comes to
`num` itself, and `false` if it comes to anything else. Numbers for which
it does are called **perfect**.

`num` is never counted in its own total. So for `num = 1` there is
nothing to add, the total is `0`, and the answer is `false`.

@examples

## Constraints

- `1 <= num <= 10^8`

## Follow-up

Trying every number below `num` can take `10^8` steps. Each divisor `d`
comes with a partner, `num / d`; can you find them all in `O(sqrt(num))`
steps?
