id: 2652
slug: sum-multiples
title: Sum Multiples
difficulty: easy
tags: math
signature: int sumOfMultiples(int n)
---
Go through the whole numbers `1, 2, ..., n`, with `n` itself included. Keep
a number when at least one of `3`, `5` and `7` divides it exactly, and drop
it otherwise. Return the total of the numbers you keep.

A number that has more than one of these divisors, such as `15`, `21`,
`35` or `105`, is still kept only once, so its value goes into the total
once. When `n` is below `3`, no number is kept and the total is `0`.

@examples

## Constraints

- `1 <= n <= 10^3`

## Follow-up

Can you find the total in `O(1)` time, without looking at the numbers one
by one?
