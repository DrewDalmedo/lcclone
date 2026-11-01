id: 3099
slug: harshad-number
title: Harshad Number
difficulty: easy
tags: math
signature: int sumOfTheDigitsOfHarshadNumber(int x)
---
Add up the decimal digits of `x`, and call the total `s`. When `s` goes
into `x` exactly, leaving no remainder, `x` is called a **Harshad
number**, and you return `s`. Otherwise return `-1`.

The value to return is the total `s`, not `x`. Each number from `1` to `9`
is a Harshad number, since its total is the number itself.

@examples

## Constraints

- `1 <= x <= 100`
