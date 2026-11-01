id: 697
slug: degree-of-an-array
title: Degree of an Array
difficulty: easy
tags: array, hash-table
signature: int findShortestSubArray(int[] nums)
---
The **degree** of a list of numbers is the number of times its most common
value occurs in it. The list `[4, 0, 4, 1, 0, 4]` has degree `3`: the value
`4` occurs three times, and no value occurs more often.

A **stretch** of `nums` is one or more elements that sit next to each
other: `nums[l], nums[l + 1], ..., nums[r]` for some `l <= r`, of length
`r - l + 1`. No stretch has a higher degree than `nums` itself, but some
reach the same degree. Return the length of the shortest stretch whose
degree equals the degree of `nums`. Only the length is asked for, not
where the stretch lies.

@examples

## Constraints

- `1 <= nums.length <= 5 * 10^4`
- `0 <= nums[i] <= 49999`

## Follow-up

An array of `n` elements has about `n^2 / 2` stretches, too many to try one
by one. Which elements must a shortest stretch begin and end with? With
that in hand, can you find its length in one pass?
