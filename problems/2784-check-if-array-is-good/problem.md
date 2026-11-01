id: 2784
slug: check-if-array-is-good
title: Check if Array is Good
difficulty: easy
tags: array, hash-table, sorting
signature: bool isGood(int[] nums)
---
Call `nums` **good** when its elements can be put in an order that reads
`1, 2, 3, ..., k - 1, k, k` for some whole number `k >= 1`. In other
words, `nums` holds every number from `1` to `k - 1` exactly once, the
number `k` exactly twice, and nothing else. For `k = 2` that order is
`1, 2, 2`; for `k = 5` it is `1, 2, 3, 4, 5, 5`.

Return `true` if `nums` is good and `false` if it is not.

The order of the elements in `nums` doesn't matter. Since such a list
has `k + 1` elements, the only `k` that can work is `nums.length - 1`.
So `nums` is never good when it has a single element: even the
shortest list, for `k = 1`, has two elements.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 200`
