id: 2696
slug: minimum-string-length-after-removing-substrings
title: Minimum String Length After Removing Substrings
difficulty: easy
tags: stack, string, simulation
signature: int minLength(string s)
---
`s` is a string of capital letters. Two letters side by side form a
**removable pair** when they are `A` directly followed by `B`, or `C`
directly followed by `D`. No other two letters are removable: not `BA` or
`DC`, not mixed pairs such as `AD` or `CB`, and nothing that contains a
letter from `E` to `Z`.

A move deletes one removable pair from the string. The two pieces left on
either side are then joined end to end, so a new removable pair can
appear where they meet. Make as many moves as you like, in any order, and
return the smallest length the string can be brought down to. It may be
`0`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of uppercase English letters.
