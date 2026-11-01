id: 2778
slug: sum-of-squares-of-special-elements
title: Sum of Squares of Special Elements
difficulty: easy
tags: array, enumeration
signature: int sumOfSquares(int[] nums)
---
Write `n` for the length of `nums`, and give its elements the numbers
`1, 2, ..., n` from left to right, so that element number `p` is
`nums[p - 1]` when indices start at `0`. Element number `p` is
**special** when `n` is a multiple of `p`: with `n = 10`, for instance,
elements number `1`, `2`, `5` and `10` are special.

Square every special element, and return the total of those squares.

The first element (number `1`) and the last (number `n`) are special
whatever `n` is. When `n = 1` they are one and the same element, and its
square is counted once.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 50`
