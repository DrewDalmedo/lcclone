id: 2283
slug: check-if-number-has-equal-digit-count-and-digit-value
title: Check if Number Has Equal Digit Count and Digit Value
difficulty: easy
tags: hash-table, string, counting
signature: bool digitCount(string num)
---
`num` is a string of `n` decimal digits, with positions numbered from `0`
to `n - 1`. Read it as a list of claims about itself: the digit standing
at position `i` claims how many characters of `num` are the digit `i`.
For instance, a `3` at position `1` claims that `num` holds exactly three
`1`s.

Return `true` if all `n` claims are correct, and `false` if at least one
is wrong.

Only the digits `0` to `n - 1` have a position making a claim about
them, so nothing is claimed about how often a larger digit occurs. A
larger digit still stands at some position, though, and makes the claim
of that position.

@examples

## Constraints

- `1 <= num.length <= 10`
- `num` consists of the characters `0` to `9` only; any digit may stand at
  any position, including a `0` at position `0`.
