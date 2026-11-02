id: 3707
slug: equal-score-substrings
title: Equal Score Substrings
difficulty: easy
tags: string, prefix-sum
signature: bool scoreBalance(string s)
---
Count through the alphabet from `a` (worth `1` point) to `z` (worth `26`
points): the count you reach at a lowercase letter is what that letter is
worth, so `c` is worth `3` and `w` is worth `23`. The **score** of a string
is the total worth of its letters, with a letter that occurs several times
counted every time: `"bob"` scores `2 + 15 + 2 = 19`.

Let `n` be the length of `s`. Pick a whole number `i` with `1 <= i < n` and
divide `s` into two substrings: its first `i` letters and its last `n - i`
letters. Both substrings keep at least one letter, and they need not be
equally long.

Return `true` if some choice of `i` gives two substrings with the same
score, and `false` if no choice does. Only the points above count, not the
letters' character codes.

@examples

## Constraints

- `2 <= s.length <= 100`
- `s` consists of lowercase English letters.
