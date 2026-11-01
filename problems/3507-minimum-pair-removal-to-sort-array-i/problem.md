id: 3507
slug: minimum-pair-removal-to-sort-array-i
title: Minimum Pair Removal to Sort Array I
difficulty: easy
tags: array, hash-table, linked-list, doubly-linked-list, ordered-set, simulation, heap-priority-queue
signature: int minimumPairRemoval(int[] nums)
---
A **fusion** takes two neighbouring elements of `nums` and puts a single
element, their sum, in their place, so the array gets one element shorter
and everything else keeps its order. Which two elements are fused is not
up to you:

- they are the neighbouring pair `nums[j]`, `nums[j + 1]` whose sum is the
  smallest among all neighbouring pairs of the array as it is now;
- a tie for the smallest sum goes to the leftmost of the tied pairs, the
  one with the smallest `j`.

Call an array **non-decreasing** when its values never go down as you
read it from left to right. Equal neighbours are fine, and an array with
a single element is non-decreasing.

Repeat this: if `nums` is non-decreasing, stop; otherwise make one fusion.
Return the number of fusions made. It is `0` when `nums` is non-decreasing
to begin with. The process always stops, because once a single element is
left the array is non-decreasing.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `-1000 <= nums[i] <= 1000`
