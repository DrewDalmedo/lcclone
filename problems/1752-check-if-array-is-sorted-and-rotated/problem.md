id: 1752
slug: check-if-array-is-sorted-and-rotated
title: Check if Array Is Sorted and Rotated
difficulty: easy
tags: array
signature: bool check(int[] nums)
---
Start from an array whose values never go down from left to right (each
value is less than or equal to the next one, so equal neighbours are
fine). Cut it into two pieces at any point, a front piece and a back
piece, and glue them together again the other way round, with the back
piece first. A cut before the first value leaves the array as it was, so a
sorted array can come out of this unchanged.

Return `true` if `nums` could have been made this way from some sorted
array, and `false` if it couldn't. Such an array, if there is one, can
only be the values of `nums` themselves, duplicates included, put in
non-decreasing order.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
