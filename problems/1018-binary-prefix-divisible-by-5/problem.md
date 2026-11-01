id: 1018
slug: binary-prefix-divisible-by-5
title: Binary Prefix Divisible By 5
difficulty: easy
tags: bit-manipulation, array
signature: bool[] prefixesDivBy5(int[] nums)
---
The array `nums` holds bits, each `0` or `1`. Feed them one at a time, from
`nums[0]` to the last, into a counter that starts at `0`. Each bit doubles
the counter's value and then adds the bit itself. So after `nums[i]` has
gone in, the counter holds the number written in binary by the digits
`nums[0]`, `nums[1]`, ..., `nums[i]`, most significant digit first. Zeros
at the front add nothing: the counter stays at `0` until the first `1`.

Return an array of booleans of the same length as `nums`, whose element `i`
is `true` when the counter's value after `nums[i]` is a multiple of `5`, and
`false` otherwise. `0` counts as a multiple of `5`.

The counter's value can have up to `10^5` binary digits, far more than any
built-in integer type holds.

@examples

## Constraints

- `1 <= nums.length <= 10^5`
- `nums[i]` is `0` or `1`.
