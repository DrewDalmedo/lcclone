id: 1763
slug: longest-nice-substring
title: Longest Nice Substring
difficulty: easy
tags: bit-manipulation, hash-table, string, divide-and-conquer, sliding-window
signature: string longestNiceSubstring(string s)
---
A string of English letters is **nice** when none of its letters is
missing its other case: for each letter of the alphabet, the string either
doesn't use that letter at all, or uses it both as a lowercase and as an
uppercase letter. How often each form occurs, and in what order, makes no
difference, so `"QqQQ"` and `"xYyX"` are nice, while `"Qqz"` is not,
since its `z` has no `Z`.

A substring of `s` is a run of one or more consecutive characters of `s`.
Return the longest substring of `s` that is nice, judged on its own
characters only: a letter whose other case appears in `s` but outside the
substring doesn't count as matched. When several nice substrings have the
greatest length, return the one that starts furthest to the left. When no
substring of `s` is nice, return the empty string `""`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of uppercase and lowercase English letters.
