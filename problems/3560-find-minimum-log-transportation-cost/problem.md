id: 3560
slug: find-minimum-log-transportation-cost
title: Find Minimum Log Transportation Cost
difficulty: easy
tags: math
signature: long minCuttingCost(int n, int m, int k)
---
Two logs, of lengths `n` and `m`, are to be moved by **three** trucks. Each
truck carries at most one piece of wood (a log left whole counts as one
piece), and only a piece whose length is at most `k`. A truck may go empty.

Before loading, you may saw the wood. Sawing a piece into two shorter pieces,
of positive whole-number lengths `a` and `b`, costs `a * b`. A piece made by
sawing may be sawn again, and the costs of all the cuts add up.

Return the least you must spend on cuts so that every piece has a truck of
its own. If both logs fit as they are, that is `0`.

Since `n` and `m` are at most `2 * k`, and at most one of them is longer than
`k`, a way to load everything always exists.

@examples

## Constraints

- `2 <= k <= 10^5`
- `1 <= n <= 2 * k`
- `1 <= m <= 2 * k`
- At most one of `n` and `m` is greater than `k`.

## Follow-up

Trying every place to cut works. Can you find the answer without trying any?
