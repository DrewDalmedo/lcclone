id: 3740
slug: minimum-distance-between-three-equal-elements-i
title: Minimum Distance Between Three Equal Elements I
difficulty: easy
tags: array, hash-table
signature: int minimumDistance(int[] nums)
---
A **trio** in `nums` is a choice of three different indices, counted from
`0`, whose elements are all equal. For a trio at indices `i`, `j` and `k`,
its **spread** sums the distances between each two of the three:

`|i - j| + |j - k| + |i - k|`

Return the smallest spread among all trios of `nums`. If no value occurs at
three or more indices, there is no trio, and the answer is `-1`.

The indices of a trio need not be adjacent: other values may sit between
them.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= nums.length`
