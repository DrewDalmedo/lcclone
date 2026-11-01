id: 496
slug: next-greater-element-i
title: Next Greater Element I
difficulty: easy
tags: stack, array, hash-table, monotonic-stack
signature: int[] nextGreaterElement(int[] nums1, int[] nums2)
---
You are given two arrays, `nums1` and `nums2`, and no number occurs twice
in `nums2`. Take a number `x` of `nums2` and read the numbers after it,
from left to right, until one of them is strictly greater than `x`: that
one is the **next greater element** of `x`. Smaller numbers on the way are
skipped, and the search ends at the first greater number, even when a
greater one still comes later. If nothing after `x` is greater than `x`,
then `x` has no next greater element.

`nums1` lists some or all of the numbers of `nums2`, each at most once and
in any order. Every number of `nums1` asks a question about `nums2`: what
is its next greater element there? Return the answers in an array lined up
with `nums1`, so that entry `i` answers for `nums1[i]`, and write `-1` for a
number that has no next greater element. The answers are numbers taken
from `nums2`, not positions in it.

@examples

## Constraints

- `1 <= nums1.length <= nums2.length <= 1000`
- `0 <= nums1[i], nums2[i] <= 10^4`
- The numbers in `nums1` are distinct, and so are the numbers in `nums2`.
- Every number in `nums1` also occurs in `nums2`.

## Follow-up

Finding each number of `nums1` and searching to its right can take about
`nums1.length * nums2.length` steps. Can you work out every answer in
`O(nums1.length + nums2.length)` time?
