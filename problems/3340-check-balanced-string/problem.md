id: 3340
slug: check-balanced-string
title: Check Balanced String
difficulty: easy
tags: string
signature: bool isBalanced(string num)
---
`num` is a string of decimal digits. Write it in a strip of squares, one
character per square, with the squares shaded alternately: the first is
light, the second dark, the third light, the fourth dark, and so on. `num`
is **balanced** when the digits on light squares add up to the same total
as the digits on dark squares.

Return `true` if `num` is balanced, and `false` if it is not.

Each character counts as the digit it shows, so `'7'` adds 7. `num` is
read as a row of separate digits, not as one number: a `0` at its start
still takes its square, and the string may be far too long for any
integer type.

@examples

## Constraints

- `2 <= num.length <= 100`
- `num` consists of the digits `'0'` to `'9'` only.
