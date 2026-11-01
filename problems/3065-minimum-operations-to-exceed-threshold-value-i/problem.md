id: 3065
slug: minimum-operations-to-exceed-threshold-value-i
title: Minimum Operations to Exceed Threshold Value I
difficulty: easy
tags: array
signature: int minOperations(int[] nums, int k)
---
A crate holds apples, and apple `i` weighs `nums[i]` grams. A sorter
clears out light apples in steps. In each step it lifts out the lightest
apple still in the crate; when several apples share that lightest weight,
the step removes only one of them.

Return the fewest steps after which every apple left in the crate weighs
at least `k` grams. An apple of exactly `k` grams is heavy enough and may
stay. At least one apple weighs `k` grams or more, so the crate never runs
empty.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 10^9`
- `1 <= k <= 10^9`
- Some `i` has `nums[i] >= k`.
