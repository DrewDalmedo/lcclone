id: 3379
slug: transformed-array
title: Transformed Array
difficulty: easy
tags: array, simulation
signature: int[] constructTransformedArray(int[] nums)
---
Number the cells of a ring from `0` to `n - 1`, where `n` is the length of
`nums`, and write `nums[i]` in cell `i`. Cells `n - 1` and `0` sit side by
side, so the ring has no ends. A step **up** adds `1` to the cell number and
a step **down** takes `1` away, except that a step up from cell `n - 1`
reaches cell `0` and a step down from cell `0` reaches cell `n - 1`.

Build an array `res` of length `n`. To find `res[i]`, place a token on cell
`i` and take `|nums[i]|` steps with it: up if `nums[i]` is positive, down if
it is negative. On the way it may pass its starting cell several times.
`res[i]` is the number written in the cell where the token ends. When
`nums[i]` is `0` the token takes no steps, so `res[i]` is `0`.

The numbers in the cells never change: every `res[i]` is read from the
original `nums`. Return `res`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `-100 <= nums[i] <= 100`
