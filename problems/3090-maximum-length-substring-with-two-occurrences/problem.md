id: 3090
slug: maximum-length-substring-with-two-occurrences
title: Maximum Length Substring With Two Occurrences
difficulty: easy
tags: hash-table, string, sliding-window
signature: int maximumLengthSubstring(string s)
---
Cut one piece out of the string `s`: pick where it starts and where it
ends, and keep every character in between, without skipping any. The piece
is **allowed** when none of its letters shows up in it three or more times.
Inside an allowed piece, each letter appears twice, once or not at all.

Return the length of the longest allowed piece.

A piece of one or two characters is always allowed, so the answer is at
least `2`.

@examples

## Constraints

- `2 <= s.length <= 100`
- `s` consists of lowercase English letters only.
