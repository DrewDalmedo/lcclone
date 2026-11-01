id: 1929
slug: concatenation-of-array
title: Concatenation of Array
difficulty: easy
tags: array, simulation
signature: int[] getConcatenation(int[] nums)
---
Write the `n` values of `nums` on a strip of paper, from left to right.
Make an exact copy of the strip and tape it on at the right end of the
first one, so that the long strip shows `nums` twice in a row.

Return the values on the long strip from left to right, as a new array of
`2n` integers. Its first `n` values are `nums` as it is, and so are its
last `n` values, in the same order: the copy is not reversed, and its
values are not mixed in among the first ones.

@examples

## Constraints

- `n == nums.length`
- `1 <= n <= 1000`
- `1 <= nums[i] <= 1000`
