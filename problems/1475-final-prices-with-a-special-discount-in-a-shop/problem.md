id: 1475
slug: final-prices-with-a-special-discount-in-a-shop
title: Final Prices With a Special Discount in a Shop
difficulty: easy
tags: stack, array, monotonic-stack
signature: int[] finalPrices(int[] prices)
---
A shop has put its goods on one long shelf, and `prices[i]` is the price tag
of item `i`, counting from the left. During a sale, each item's tag is cut by
an amount that depends on the items to its right:

- Start at item `i` and walk right, one item at a time. Stop at the first
  item whose tag is **at most** the tag of item `i` (equal tags count). That
  item is the **partner** of item `i`. Items with higher tags that you pass
  on the way don't matter.
- Item `i` then sells for its own tag minus its partner's tag, so an item
  whose partner has the same tag sells for `0`.
- If no item to the right of item `i` has a tag that low, item `i` has no
  partner and sells for its full tag. The rightmost item never has one.

Only the nearest partner counts, even when a cheaper item lies further to
the right. The items don't affect each other: each one uses the tags as
they are written in `prices`, and one item can be the partner of several
others.

Return an array as long as `prices` whose entry `i` is the price item `i`
sells for.

@examples

## Constraints

- `1 <= prices.length <= 500`
- `1 <= prices[i] <= 1000`
