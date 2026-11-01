id: 1748
slug: sum-of-unique-elements
title: Sum of Unique Elements
difficulty: easy
tags: array, hash-table, counting
signature: int sumOfUnique(int[] nums)
---
Go through the integer array `nums` and strike out every value that is
written in it more than once. Strike out all of its copies, not just the
extra ones: a value that appears twice, three times or more leaves nothing
behind.

Return the sum of the entries that are left. These are the values that
appear in `nums` exactly once. If every entry has been struck out, the sum
is `0`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
