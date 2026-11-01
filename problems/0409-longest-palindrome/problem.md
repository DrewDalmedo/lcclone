id: 409
slug: longest-palindrome
title: Longest Palindrome
difficulty: easy
tags: greedy, hash-table, string
signature: int longestPalindrome(string s)
---
Think of the string `s` as a pile of letter tiles, one tile for each of its
characters. Pick some of the tiles, as many or as few as you like, and lay
them out in a row that reads the same from right to left as from left to
right. Each tile can be used at most once, and tiles may be left over.

Return the length of the longest row you can make this way. Only the length
is asked for, not the row itself.

An uppercase letter and its lowercase form are different tiles: a `K` and a
`k` never mirror each other. A single tile on its own reads the same both
ways, so the answer is always at least 1.

@examples

## Constraints

- `1 <= s.length <= 2000`
- `s` consists of uppercase and lowercase English letters only.
