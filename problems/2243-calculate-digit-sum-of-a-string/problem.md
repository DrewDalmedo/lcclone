id: 2243
slug: calculate-digit-sum-of-a-string
title: Calculate Digit Sum of a String
difficulty: easy
tags: string, simulation
signature: string digitSum(string s, int k)
---
`s` is a string of decimal digits and `k` is a block size. Shrink `s` by
making passes over it, for as long as it has **more than** `k` characters.
One pass does this:

1. Starting from the left, chop `s` into blocks of `k` characters each. The
   last block takes whatever is left over, so it can be shorter than `k`.
2. Add up the digits of each block, and write that total in ordinary
   decimal: no leading zeros, and a total of zero written as `"0"`.
3. Join those totals together, in the order of their blocks. The result is
   the new `s`.

When `s` has `k` characters or fewer, stop and return it. This test comes
before the first pass too: if `s` starts out no longer than `k`, return it
exactly as it is, leading zeros included.

`s` is only ever handled as text, never as one big number, so zeros at its
front are kept.

@examples

## Constraints

- `1 <= s.length <= 100`
- `2 <= k <= 100`
- `s` consists of the digits `0` to `9` only.
