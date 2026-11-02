id: 3550
slug: smallest-index-with-digit-sum-equal-to-index
title: Smallest Index With Digit Sum Equal to Index
difficulty: easy
tags: array, math
signature: int smallestIndex(int[] nums)
---
Number the positions of `nums` from `0`. To get an element's **digit
sum**, write it in decimal and add its digits together: for `586` that
is `5 + 8 + 6 = 19`, for `1000` it is `1`, and for `0` it is `0`.

A position `i` is a **match** when the element stored there has digit
sum exactly `i`. Return the lowest position that is a match, or `-1` if
`nums` has no match at all.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `0 <= nums[i] <= 1000`
