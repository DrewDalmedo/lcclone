id: 2160
slug: minimum-sum-of-four-digit-number-after-splitting-digits
title: Minimum Sum of Four Digit Number After Splitting Digits
difficulty: easy
tags: greedy, math, sorting
signature: int minimumSum(int num)
---
`num` is written with exactly four decimal digits. Share those four digits
out between two new numbers `a` and `b`:

- every digit of `num` goes to exactly one of the two, so a digit that
  occurs twice in `num` is used twice, and none is left over;
- `a` and `b` each get at least one digit;
- you choose the order in which each number's digits are written.

A number may begin with `0`: the digits `0` and `6`, written in that order,
make `6`, and `0` followed by `0` makes `0`.

Return the smallest value that `a + b` can take.

@examples

## Constraints

- `1000 <= num <= 9999`
