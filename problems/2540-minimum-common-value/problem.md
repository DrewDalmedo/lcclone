id: 2540
slug: minimum-common-value
title: Minimum Common Value
difficulty: easy
tags: array, hash-table, two-pointers, binary-search
signature: int getCommon(int[] nums1, int[] nums2)
---
Two sensors log the times at which they fire. `nums1` holds the timestamps
logged by the first sensor and `nums2` those logged by the second, each in
non-decreasing order. A sensor may log the same timestamp more than once.

Return the earliest timestamp found in both logs; how many times each log
repeats it doesn't matter. If no timestamp is in both logs, return `-1`.

@examples

## Constraints

- `1 <= nums1.length, nums2.length <= 10^5`
- `1 <= nums1[i], nums2[j] <= 10^9`
- `nums1` and `nums2` are each sorted in non-decreasing order.

## Follow-up

Comparing every timestamp of one log with every timestamp of the other
takes `O(n * m)` time. Since both logs are sorted, can you find the answer
in a single pass over each?
