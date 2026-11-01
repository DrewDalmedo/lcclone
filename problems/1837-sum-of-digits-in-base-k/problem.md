id: 1837
slug: sum-of-digits-in-base-k
title: Sum of Digits in Base K
difficulty: easy
tags: math
signature: int sumBase(int n, int k)
---
An abacus has rods numbered `0`, `1`, `2`, ... from right to left. A bead
on rod `i` is worth `k^i`, and no rod may hold more than `k - 1` beads.
Under that limit, every positive whole number can be shown in exactly one
way: the number of beads on rod `i` is then the digit in the `k^i` place
when the number is written in base `k`.

Set the abacus to show `n`, and return how many beads it holds in all.
In other words, return the sum of the base-`k` digits of `n`, as an
ordinary integer. Since `k` is at most `10`, every such digit is one of
`0` to `9`.

@examples

## Constraints

- `1 <= n <= 100`
- `2 <= k <= 10`
