id: 2706
slug: buy-two-chocolates
title: Buy Two Chocolates
difficulty: easy
tags: greedy, array, sorting
signature: int buyChoco(int[] prices, int money)
---
`prices[i]` is the price of chocolate bar `i` on a shop's shelf, and
`money` is the cash you walk in with. You want to leave with exactly
two bars: two separate bars, not one bar counted twice, though the two may
have the same price.

Your cash can't drop below zero, so the two prices together may come to
at most `money`; spending every last coin is fine. Out of the pairs you can
afford, buy the cheapest one, and return the cash you are left holding.

If no pair of bars fits within `money`, you buy nothing at all; return
`money` unchanged.

@examples

## Constraints

- `2 <= prices.length <= 50`
- `1 <= prices[i] <= 100`
- `1 <= money <= 100`
