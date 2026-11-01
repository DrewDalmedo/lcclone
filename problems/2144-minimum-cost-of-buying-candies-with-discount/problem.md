id: 2144
slug: minimum-cost-of-buying-candies-with-discount
title: Minimum Cost of Buying Candies With Discount
difficulty: easy
tags: greedy, array, sorting
signature: int minimumCost(int[] cost)
---
You are taking home every candy listed in `cost`, where `cost[i]` is the
price of candy `i`. The shop has a deal on: at the till, you group your
candies into bundles, with no candy left out and none in two bundles,
and each bundle must be one of these:

- one candy, which you pay for;
- two candies, both of which you pay for;
- three candies, of which you pay for two, while the third is free. The
  free one must not be dearer than either of the two you pay for; a price
  equal to the cheaper paid price is fine.

How the candies are bundled is up to you. Return the smallest total you
can pay for all of them.

@examples

## Constraints

- `1 <= cost.length <= 100`
- `1 <= cost[i] <= 100`
