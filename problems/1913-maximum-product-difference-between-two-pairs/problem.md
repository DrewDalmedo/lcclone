id: 1913
slug: maximum-product-difference-between-two-pairs
title: Maximum Product Difference Between Two Pairs
difficulty: easy
tags: array, sorting
signature: int maxProductDifference(int[] nums)
---
Choose four different positions in `nums` and split the four values found
there into two pairs: a **plus** pair and a **minus** pair. Multiply the
two values of each pair, then subtract the minus pair's product from the
plus pair's product. The result is the **product difference** of the two
pairs.

Return the largest product difference that any such choice gives.

No position may be chosen twice. Values, on the other hand, may repeat: if
`nums` holds the same number at two positions, choosing both positions gives
you that number twice, and the two copies may even make up one pair.

@examples

## Constraints

- `4 <= nums.length <= 10^4`
- `1 <= nums[i] <= 10^4`
