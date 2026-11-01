id: 2395
slug: find-subarrays-with-equal-sum
title: Find Subarrays With Equal Sum
difficulty: easy
tags: array, hash-table
signature: bool findSubarrays(int[] nums)
---
Every index `i` of `nums` except the last has a right-hand neighbour, and
`nums[i] + nums[i + 1]` is the **neighbour sum** at `i`. So an array of `n`
numbers has `n - 1` neighbour sums, at the indices `0` to `n - 2`; each is
the sum of a subarray of length 2.

Return `true` if the neighbour sums at two different indices are equal, and
`false` if all `n - 1` of them are different.

The two indices may be next to each other, in which case the two subarrays
share an element: in `[5, 0, 5]` the neighbour sums at `0` and `1` are both
`5`. The numbers added up may differ too: in `[1, 6, 4, 3]` the sums at `0`
and `2` are both `7`.

@examples

## Constraints

- `2 <= nums.length <= 1000`
- `-10^9 <= nums[i] <= 10^9`
