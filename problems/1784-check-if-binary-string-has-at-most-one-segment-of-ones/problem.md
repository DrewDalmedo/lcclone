id: 1784
slug: check-if-binary-string-has-at-most-one-segment-of-ones
title: Check if Binary String Has at Most One Segment of Ones
difficulty: easy
tags: string
signature: bool checkOnesSegment(string s)
---
`s` is made of the characters `0` and `1`, and its first character is
always `1`. Cut `s` into **runs**: pieces of equal characters, each as long
as it can be, so that two neighbouring runs never hold the same character.
For example, `"110100"` has the runs `11`, `0`, `1` and `00`. A single `1` between
two `0`s, or at an end of `s`, is a run of its own.

Return `true` if `s` has at most one run of `1`s, and `false` if it has two
or more. Since `s` starts with `1`, it always has at least one.

@examples

## Constraints

- `1 <= s.length <= 100`
- Each character of `s` is `0` or `1`.
- `s[0]` is `1`.
