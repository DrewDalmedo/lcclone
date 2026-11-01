id: 338
slug: counting-bits
title: Counting Bits
difficulty: easy
tags: bit-manipulation, dynamic-programming
signature: int[] countBits(int n)
---
Take every whole number from `0` up to `n`, both ends included, and write
it in binary. Return an array of `n + 1` counts, one per number in that
order: the count for a number tallies the `1`s that appear when it is
written that way.

So the array always starts with the count for `0`, which is `0`, and ends
with the count for `n`. For example, `6` is `110` in binary, so the count
at index `6` is `2`.

Work the counts out yourself, rather than asking a built-in bit counter
such as `__builtin_popcount` for them.

@examples

## Constraints

- `0 <= n <= 10^5`

## Follow-up

Counting the digits of each number on its own takes about `log n` steps
per number. Can you fill the array in `O(n)` time, in a single pass, by
reusing counts already in it?
