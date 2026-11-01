id: 1844
slug: replace-all-digits-with-characters
title: Replace All Digits with Characters
difficulty: easy
tags: string
signature: string replaceDigits(string s)
---
The string `s` alternates between letters and digits: the characters at
even indices (`0`, `2`, `4`, ...) are lowercase letters, and those at odd
indices are digits from `0` to `9`.

Each digit is shorthand for a letter. The digit `d` at index `i` stands
for the letter `d` places further on in the alphabet than `s[i - 1]`, the
letter just before it. So a `0` repeats the letter before it, and a `3`
after `k` stands for `n`.

Return the string you get when every digit of `s` is written out as the
letter it stands for, and the letters of `s` stay where they are. A digit
always counts on from the letter of `s` right before it, never from the
letter another digit became, and it never has to count past `z`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s[i]` is a lowercase English letter for every even `i`, and a digit
  for every odd `i`.
- For every odd index `i`, counting `s[i]` places on from `s[i - 1]` stays
  within `a` to `z`.
