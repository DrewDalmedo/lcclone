id: 3731
slug: find-missing-elements
title: Find Missing Elements
difficulty: easy
tags: array, hash-table, sorting
signature: int[] findMissingElements(int[] nums)
---
Mark each value of `nums` on a number line. The values are all different
and come in no particular order. The marks stretch from the smallest
value, `lo`, to the largest, `hi`.

Return, in increasing order, the unmarked integers that lie between `lo`
and `hi`. Both `lo` and `hi` carry marks, so neither can be in the answer,
and nothing below `lo` or above `hi` counts. When the marks leave no gap,
return an empty array.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
- The values of `nums` are all different.
