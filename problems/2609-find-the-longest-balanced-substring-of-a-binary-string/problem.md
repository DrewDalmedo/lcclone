id: 2609
slug: find-the-longest-balanced-substring-of-a-binary-string
title: Find the Longest Balanced Substring of a Binary String
difficulty: easy
tags: string
signature: int findTheLongestBalancedSubstring(string s)
---
Every character of `s` is `0` or `1`. A string is **balanced** when, for
some `k >= 0`, it is made of `k` `0`s and then `k` `1`s and nothing else:
the empty string (`k = 0`), `"01"`, `"0011"`, `"000111"` and so on. So
`"10"` and `"1100"` are not balanced (the `1`s come first), nor is
`"001011"` (the two characters are mixed) or `"001"` (one `0` too many).

Find the longest balanced substring of `s`, that is, the longest balanced
stretch of consecutive characters of `s`, and return its length. The empty
substring is balanced, so the answer is `0` if `s` has no other balanced
substring.

@examples

## Constraints

- `1 <= s.length <= 50`
- Each character of `s` is `0` or `1`.
