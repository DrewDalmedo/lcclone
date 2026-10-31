id: 448
slug: find-all-numbers-disappeared-in-an-array
title: Find All Numbers Disappeared in an Array
difficulty: easy
tags: array, hash-table
signature: int[] findDisappearedNumbers(int[] nums)
compare: unordered 0
---
`nums` holds `n` integers, where `n` is its length, and each of them is
between `1` and `n` inclusive. Some values may be repeated, which leaves
others out.

Return every integer from `1` to `n` that occurs nowhere in `nums`, each
listed once. The integers may be returned *in any order*. If every integer
from `1` to `n` occurs in `nums`, return an empty array.

@examples

## Constraints

- `n == nums.length`
- `1 <= n <= 10^5`
- `1 <= nums[i] <= n`

## Follow-up

Every value of `nums` is also a valid position in it, once you subtract 1.
Can you use that to find the answer in `O(n)` time, with `O(1)` memory
apart from the array you return?
