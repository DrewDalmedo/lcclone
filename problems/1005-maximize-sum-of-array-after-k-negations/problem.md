id: 1005
slug: maximize-sum-of-array-after-k-negations
title: Maximize Sum Of Array After K Negations
difficulty: easy
tags: greedy, array, sorting
signature: int largestSumAfterKNegations(int[] nums, int k)
---
A **flip** changes the sign of one element of `nums`: it turns `nums[i]`
into `-nums[i]`. You have to make exactly `k` flips, one after another, and
you choose which element each of them hits. The same element may be hit as
often as you like; flipping it twice puts it back as it was.

Once all `k` flips are made, add up the elements of `nums`. Return the
largest total you can end up with. All `k` flips are compulsory, even when
stopping earlier would leave a larger total.

@examples

## Constraints

- `1 <= nums.length <= 10^4`
- `-100 <= nums[i] <= 100`
- `1 <= k <= 10^4`
