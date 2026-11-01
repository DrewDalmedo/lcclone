id: 2600
slug: k-items-with-the-maximum-sum
title: K Items With the Maximum Sum
difficulty: easy
tags: greedy, math
signature: int kItemsWithMaximumSum(int numOnes, int numZeros, int numNegOnes, int k)
---
A pile of chips lies on the table. `numOnes` of them score `+1` point each,
`numZeros` score nothing, and `numNegOnes` score `-1` point each.

You must pick up exactly `k` different chips: not fewer, even when that
forces you to take some that cost a point. Your score is the sum of the
points on the chips you picked. Return the highest score you can get.

With `k = 0` you pick nothing, and the score is `0`.

@examples

## Constraints

- `0 <= numOnes, numZeros, numNegOnes <= 50`
- `0 <= k <= numOnes + numZeros + numNegOnes`
