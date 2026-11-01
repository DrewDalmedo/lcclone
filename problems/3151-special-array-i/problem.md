id: 3151
slug: special-array-i
title: Special Array I
difficulty: easy
tags: array
signature: bool isArraySpecial(int[] nums)
---
Call two numbers **mismatched** when exactly one of the two is odd. `nums`
has `n` elements, and so `n - 1` pairs of neighbours: `nums[i]` and
`nums[i + 1]`, for each `i` from `0` to `n - 2`. Return `true` if every one
of those pairs is mismatched, and `false` if some pair is made of two odd
numbers or of two even numbers.

Read from left to right, then, the elements have to switch between odd and
even at every step, and the first element may be of either kind. An array
of a single element has no neighbouring pair to check, so its answer is
`true`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
