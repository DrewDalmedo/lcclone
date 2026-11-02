id: 3745
slug: maximize-expression-of-three-elements
title: Maximize Expression of Three Elements
difficulty: easy
tags: greedy, array, enumeration, sorting
signature: int maximizeExpressionOfThree(int[] nums)
---
Fill in the expression `a + b - c` with numbers read from three different
positions of `nums`, one position for each letter. Return the largest value
the expression can reach.

The positions may lie in any order in `nums`, and the same number may be
used more than once if it is stored at more than one position. For
instance, when the largest number appears twice, its two copies can serve
as `a` and `b`; when it appears only once, only one of `a` and `b` can be
it.

@examples

## Constraints

- `3 <= nums.length <= 100`
- `-100 <= nums[i] <= 100`
