id: 3442
slug: maximum-difference-between-even-and-odd-frequency-i
title: Maximum Difference Between Even and Odd Frequency I
difficulty: easy
tags: hash-table, string, counting
signature: int maxDifference(string s)
---
Count how many times each letter occurs in `s`, and call that number the
letter's **tally**. Only the letters that actually occur in `s` have a
tally: a letter missing from `s` is left out altogether, so its zero is
not an even tally you can use.

Choose two letters of `s`, one whose tally is odd and one whose tally is
even, and subtract the even tally from the odd one. Return the largest
result that any such choice gives.

The result is negative when every odd tally is smaller than every even
one, and that is a valid answer too.

@examples

## Constraints

- `3 <= s.length <= 100`
- `s` consists of lowercase English letters.
- At least one letter of `s` has an odd tally, and at least one has an
  even tally.
