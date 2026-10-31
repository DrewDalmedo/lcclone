id: 643
slug: maximum-average-subarray-i
title: Maximum Average Subarray I
difficulty: easy
tags: array, sliding-window
signature: double findMaxAverage(int[] nums, int k)
---
Among all runs of exactly `k` consecutive elements of `nums`, find the one
whose average is largest, and return that average.

An answer within `10^-5` of the exact average is accepted.

@examples

## Constraints

- `1 <= k <= nums.length <= 10^5`
- `-10^4 <= nums[i] <= 10^4`

## Follow-up

Adding up every run from scratch takes `O(n * k)` time. When a run moves one
place to the right, how does its sum change?
