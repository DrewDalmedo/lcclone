id: 3258
slug: count-substrings-that-satisfy-k-constraint-i
title: Count Substrings That Satisfy K-Constraint I
difficulty: easy
tags: string, sliding-window
signature: int countKConstraintSubstrings(string s, int k)
---
`s` is a string of the characters `'0'` and `'1'`. In any such string, call
the character with the smaller count the **minority**; when both counts are
equal, either one is. A string meets the **k-constraint** when its minority
occurs no more than `k` times. Put another way: it has at most `k` zeros,
or at most `k` ones, or both. A string of one repeated character always
meets it, since the other character occurs 0 times.

Return how many substrings of `s` meet the k-constraint. A substring is a
non-empty run of consecutive characters of `s`, and substrings are told
apart by where they start and end: two of them at different places both
count, even when they read the same.

@examples

## Constraints

- `1 <= s.length <= 50`
- `1 <= k <= s.length`
- Each `s[i]` is `'0'` or `'1'`.
