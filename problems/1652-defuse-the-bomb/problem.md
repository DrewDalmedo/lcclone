id: 1652
slug: defuse-the-bomb
title: Defuse the Bomb
difficulty: easy
tags: array, sliding-window
signature: int[] decrypt(int[] code, int k)
---
Picture the `n` numbers of `code` placed around a circle in index order.
Stepping **forward** from `code[i]` leads to `code[i + 1]`, and stepping
forward from the last number, `code[n - 1]`, leads round to `code[0]`.
Stepping **backward** goes the other way, so the step back from `code[0]`
lands on `code[n - 1]`.

Return a new array `res` of length `n`. Its entry `res[i]` is found by
starting at position `i` and adding up the numbers you step onto:

- if `k > 0`, take `k` steps forward and add up the `k` numbers reached;
- if `k < 0`, take `-k` steps backward and add up the `-k` numbers reached;
- if `k = 0`, take no steps, so `res[i]` is `0`.

The number at the starting position is not counted, and since fewer than `n`
steps are taken, the walk never comes back to it. Every entry of `res` is
worked out from `code` as it was given, never from other entries of `res`.

@examples

## Constraints

- `n == code.length`
- `1 <= n <= 100`
- `1 <= code[i] <= 100`
- `-(n - 1) <= k <= n - 1`
