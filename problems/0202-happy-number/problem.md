id: 202
slug: happy-number
title: Happy Number
difficulty: easy
tags: hash-table, math, two-pointers
signature: bool isHappy(int n)
---
For a positive integer `x`, let `s(x)` be what you get by squaring each
decimal digit of `x` and adding the squares up. For instance,
`s(305) = 9 + 0 + 25 = 34`.

Applying `s` to `n` again and again gives the sequence `n`, `s(n)`,
`s(s(n))`, and so on. Call `n` **happy** if `1` turns up somewhere in that
sequence. In particular `n = 1` is happy, as the sequence starts with it.
Because `s(1) = 1`, every term after a `1` is `1` as well.

When `1` never turns up, the terms can't all be different: at some point a
term equals an earlier one, and from there the sequence cycles through the
same values without end.

Return `true` if `n` is happy and `false` if it is not.

@examples

## Constraints

- `1 <= n <= 2^31 - 1`

## Follow-up

Storing every term seen so far is one way to notice the cycle. Can you
notice it with only `O(1)` extra memory?
