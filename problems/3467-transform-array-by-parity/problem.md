id: 3467
slug: transform-array-by-parity
title: Transform Array by Parity
difficulty: easy
tags: array, counting, sorting
signature: int[] transformArray(int[] nums)
---
Give every element of `nums` a **parity mark**: `0` if the element is
even, `1` if it is odd. Collect the marks, one per element, and arrange
them in non-decreasing order, so that every `0` comes before every `1`.
Return that array of marks.

The result has as many entries as `nums`. It is the marks that are
sorted, not the elements themselves, so the result depends only on how
many elements are even and how many are odd.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 1000`
