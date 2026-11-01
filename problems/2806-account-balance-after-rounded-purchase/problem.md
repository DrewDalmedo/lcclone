id: 2806
slug: account-balance-after-rounded-purchase
title: Account Balance After Rounded Purchase
difficulty: easy
tags: math
signature: int accountBalanceAfterPurchase(int purchaseAmount)
---
A prepaid card holds a balance of `100`. You use it to buy one item priced
at `purchaseAmount`, but the shop's till only charges amounts that are
multiples of `10`. It rounds the price by its last digit:

- last digit `0` to `4`: the price is rounded **down**, to the multiple of
  `10` at or just below it. `43` is charged as `40`, a multiple of `10`
  stays as it is, and a price from `0` to `4` is charged as `0`, so the
  item is free.
- last digit `5` to `9`: the price is rounded **up** to the next multiple
  of `10`. `47` is charged as `50`. A price ending in `5` sits exactly
  halfway between two multiples, and it is rounded up as well: `65` is
  charged as `70`.

Return the balance left on the card once the rounded price has been taken
off.

@examples

## Constraints

- `0 <= purchaseAmount <= 100`
