id: 2697
slug: lexicographically-smallest-palindrome
title: Lexicographically Smallest Palindrome
difficulty: easy
tags: greedy, two-pointers, string
signature: string makeSmallestPalindrome(string s)
---
You may edit the string `s` one letter at a time. An **edit** picks one
position of `s` and writes any lowercase English letter there in place of
the old one.

Use as few edits as possible to turn `s` into a **palindrome**, a string
equal to its own reverse. Often several palindromes can be reached with
that smallest number of edits; return the one that comes first in
dictionary order. To put two strings of equal length in that order, read
them from the left up to the first place where they disagree: the string
holding the letter nearer the start of the alphabet there goes first.

The number of edits is settled first, and dictionary order only chooses
among the palindromes that need exactly that many. Spending an extra edit
to bring in an earlier letter is never allowed, so a string that is
already a palindrome comes back unchanged.

@examples

## Constraints

- `1 <= s.length <= 1000`
- `s` consists of lowercase English letters only.
