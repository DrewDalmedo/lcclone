id: 2716
slug: minimize-string-length
title: Minimize String Length
difficulty: easy
tags: hash-table, string
signature: int minimizedStringLength(string s)
---
You may shorten the string `s` with the following move, as often as you
like, or not at all:

1. Point at any character of `s`, and call its letter `c`.
2. Choose a direction, left or right.
3. Erase the copy of `c` nearest to the character you pointed at in that
   direction, however many other characters lie between the two. The
   character you pointed at stays. If no other `c` lies in that direction,
   nothing is erased.

After each move the remaining characters close up, and the next move is
made on the shorter string. Return the smallest length that `s` can be
brought down to.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters only.
