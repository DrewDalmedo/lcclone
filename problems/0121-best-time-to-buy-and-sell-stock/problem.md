id: 121
slug: best-time-to-buy-and-sell-stock
title: Best Time to Buy and Sell Stock
difficulty: easy
tags: array, dynamic-programming
signature: int maxProfit(int[] prices)
---
`prices[i]` is the price of a stock on day `i`. You may buy one share on one
day and sell it on a **later** day. Return the largest profit (the selling
price minus the buying price) you can make this way.

If every possible sale would lose money or break even, don't trade at all,
and return `0`.

@examples

## Constraints

- `1 <= prices.length <= 10^5`
- `0 <= prices[i] <= 10^4`

## Follow-up

Comparing every pair of days takes `O(n^2)` time. What do you need to
remember about the days before day `i` to know the best sale on day `i`?
