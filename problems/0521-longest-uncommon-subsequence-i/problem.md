id: 521
slug: longest-uncommon-subsequence-i
title: Longest Uncommon Subsequence I
difficulty: easy
tags: string
signature: int findLUSlength(string a, string b)
---
Let `t` be a string of `k` characters. It is a **subsequence** of a string
`s` when `s` has `k` positions `p1 < p2 < ... < pk` such that `s[p1]` is the
first character of `t`, `s[p2]` the second one, and so on up to `s[pk]`,
its last. Every string is a subsequence of itself, and the empty string
(with `k = 0`) is a subsequence of every string. For instance, `"one"` and
`"key"` are subsequences of `"monkey"`, but `"yen"` is not: `"monkey"` has
those letters, though not in that order.

You are given two strings, `a` and `b`. A string is an **uncommon
subsequence** of the two when it is a subsequence of one of them but not
of the other. Find an uncommon subsequence with as many characters as
possible and return how many characters it has. If no string is an
uncommon subsequence of `a` and `b`, return `-1`.

@examples

## Constraints

- `1 <= a.length, b.length <= 100`
- `a` and `b` consist of lowercase English letters.
