id: 4048
slug: count-values-with-equally-spaced-occurrences-i
title: Count Values With Equally Spaced Occurrences I
difficulty: easy
tags: array, hash-table
signature: int countSpecialIntegers(int[] nums)
---
A value of `nums` is **equally spaced** when both of these are true:

- it is written in `nums` exactly three times, no more and no fewer;
- its middle copy sits exactly halfway between the other two. With the
  three copies at indices `a < b < c`, that means `b - a == c - b`.

The distances are differences of indices, whatever values stand in between.
Three copies side by side, at `a`, `a + 1` and `a + 2`, are equally spaced.
A value written four or more times never is, not even when three of its
copies are evenly apart, and neither is a value written once or twice.

Return how many different values of `nums` are equally spaced. Each one
counts once, not once for each of its copies.

@examples

## Constraints

- `3 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
