id: 724
slug: find-pivot-index
title: Find Pivot Index
difficulty: easy
tags: array, prefix-sum
signature: int pivotIndex(int[] nums)
---
Picture the elements of `nums` laid in order along a beam, and the beam
resting on a pivot placed under one of them, element `i`. That element sits
on the pivot and counts for neither side. The elements before it, `nums[0]`
to `nums[i - 1]`, make up the left arm; the elements after it, `nums[i + 1]`
to the last one, make up the right arm. The beam **balances** at `i`, and
`i` is a **pivot index**, when the two arms have the same sum. Only these
sums count, not how far an element lies from the pivot, and a negative
element simply lowers the sum of its arm.

An arm with no elements sums to `0`. So the beam balances at index `0` when
the elements after it add up to `0`, and at the last index when the
elements before it do.

Return the smallest pivot index of `nums`, or `-1` if the beam balances at
no index.

@examples

## Constraints

- `1 <= nums.length <= 10^4`
- `-1000 <= nums[i] <= 1000`

## Follow-up

Recounting both arms at every index costs `O(n^2)` time. If you know the
total of `nums` and carry the left arm's sum along as you move right, can
you settle each index in `O(1)`?
