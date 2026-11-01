id: 1332
slug: remove-palindromic-subsequences
title: Remove Palindromic Subsequences
difficulty: easy
tags: two-pointers, string
signature: int removePalindromeSub(string s)
---
The string `s` is made of the letters `a` and `b` only, and you want to
erase all of it. In one **move** you mark some of the letters that are
still there, as many as you like and not necessarily side by side, as long
as the marked letters, taken from left to right, spell a palindrome. The
marked letters are all erased at once, and the letters that remain close
up without changing their order.

A palindrome is a string that is unchanged when it is reversed, so a single
letter is one too.

Return the smallest number of moves after which nothing is left of `s`.

@examples

## Constraints

- `1 <= s.length <= 1000`
- `s` consists only of the lowercase letters `a` and `b`.
