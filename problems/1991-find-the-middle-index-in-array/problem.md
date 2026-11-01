id: 1991
slug: find-the-middle-index-in-array
title: Find the Middle Index in Array
difficulty: easy
tags: array, prefix-sum
signature: int findMiddleIndex(int[] nums)
---
Let `n` be the length of `nums`. For an index `i` from `0` to `n - 1`,
write `before(i)` for the sum of the elements in front of it,
`nums[0] + ... + nums[i - 1]`, and `after(i)` for the sum of the elements
behind it, `nums[i + 1] + ... + nums[n - 1]`. Neither sum includes
`nums[i]` itself. A sum of no elements is `0`, so `before(0)` and
`after(n - 1)` are both `0`.

An index `i` is a **middle index** when `before(i) == after(i)`. Return the
smallest middle index of `nums`, or `-1` if no index is one. The elements
may be negative or zero.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `-1000 <= nums[i] <= 1000`
