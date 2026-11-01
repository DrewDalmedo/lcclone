id: 441
slug: arranging-coins
title: Arranging Coins
difficulty: easy
tags: math, binary-search
signature: int arrangeCoins(int n)
---
You lay `n` coins out on a table in rows, one row under another. Row `1`
holds a single coin, row `2` holds two, and in general row `r` holds `r`
coins. You start a row only once the row above it is full, and keep going
until the coins run out.

When you stop, the last row you started may be missing some coins, or every
row you started may be full. Return how many rows are **full**.

In other words, return the largest whole number `k` such that
`1 + 2 + ... + k`, which is `k * (k + 1) / 2`, is at most `n`.

@examples

## Constraints

- `1 <= n <= 2^31 - 1`

## Follow-up

Filling the rows one by one takes 65,535 rounds for the largest `n`. Can
you find the answer in `O(log n)` steps, without overflowing a 32-bit
`int` along the way?
