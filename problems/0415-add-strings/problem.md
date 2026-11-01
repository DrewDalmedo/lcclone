id: 415
slug: add-strings
title: Add Strings
difficulty: easy
tags: math, string, simulation
signature: string addStrings(string num1, string num2)
---
Add two numbers that can be far too long for any integer type. Each of
`num1` and `num2` is a non-negative integer spelled out in decimal, one
character per digit, with the highest place first. Return the decimal
spelling of `num1 + num2`, in the same style.

A number may have up to `10^4` digits, so reading a whole string into an
`int`, a `long long` or even a `double` overflows or loses digits. Work
through the digits instead, the way column addition is done by hand:
rightmost column first, carrying a 1 into the next column whenever a column
adds up to 10 or more.

The answer must be spelled exactly: digits only, with no `+` sign, no
spaces and no leading zeros. The one number that starts with `0` is zero
itself, spelled `"0"`.

@examples

## Constraints

- `1 <= num1.length, num2.length <= 10^4`
- `num1` and `num2` consist of digits only.
- Neither number has a leading zero, except the number zero itself, which is
  written `"0"`.
