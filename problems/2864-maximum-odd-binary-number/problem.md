id: 2864
slug: maximum-odd-binary-number
title: Maximum Odd Binary Number
difficulty: easy
tags: greedy, math, string
signature: string maximumOddBinaryNumber(string s)
---
`s` is a string of the characters `'0'` and `'1'`, with at least one `'1'`
in it. Arrange its characters in any order, using each of them exactly
once (the order they already have is one of the choices), and read the
result as a number written in base 2, leftmost digit first. The number is
odd exactly when the last character is `'1'`.

Of all the arrangements that make an odd number, return the one whose
number is the largest.

The answer is as long as `s`, with as many `'0'`s and as many `'1'`s as
`s`. Zeros at its front are part of the answer and stay there: for
`"10000"` the answer is `"00001"`, not `"1"`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists only of `'0'` and `'1'`.
- `s` contains at least one `'1'`.
