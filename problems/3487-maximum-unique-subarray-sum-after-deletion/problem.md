id: 3487
slug: maximum-unique-subarray-sum-after-deletion
title: Maximum Unique Subarray Sum After Deletion
difficulty: easy
tags: greedy, array, hash-table
signature: int maxSum(int[] nums)
---
Starting from the integer array `nums`, you make two moves, in this order.

1. Erase any of its entries you like, as many as you like or none at all,
   as long as at least one entry survives. The survivors close up, keeping
   their order.
2. Among the survivors, pick a **block**: one or more of them that sit next
   to each other, with no value occurring twice in the block. The block
   may be all the survivors.

Your score is the sum of the block. Return the highest score you can reach.

The block always holds at least one entry, so when every entry of `nums`
is negative, the score is negative too.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `-100 <= nums[i] <= 100`
