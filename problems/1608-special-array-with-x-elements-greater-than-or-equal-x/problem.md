id: 1608
slug: special-array-with-x-elements-greater-than-or-equal-x
title: Special Array With X Elements Greater Than or Equal X
difficulty: easy
tags: array, binary-search, sorting
signature: int specialArray(int[] nums)
---
For a whole number `x >= 0`, count the elements of `nums` that are at least
`x` (an element equal to `x` counts). Call `x` a **match** for `nums` when
that count is exactly `x`.

Return the match if `nums` has one, and `-1` if it has none. Any whole number
can be the match, whether or not it appears in `nums`. `nums` never has more
than one match, since raising `x` can only lower the count or leave it as it
was.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `0 <= nums[i] <= 1000`
