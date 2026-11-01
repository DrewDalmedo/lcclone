id: 3210
slug: find-the-encrypted-string
title: Find the Encrypted String
difficulty: easy
tags: string
signature: string getEncryptedString(string s, int k)
---
A message `s` of lowercase letters is disguised by moving its letters to new
places, never by changing them. Let `n` be the length of `s`, with positions
numbered from `0` to `n - 1`. The disguised message `t` also has `n`
letters, and its letter at position `i` is the one
found `k` places to the right of position `i` in `s`. Counting places to the
right carries on past the last letter of `s` and continues from its first
letter, as often as needed, so position `i` of `t` holds `s[(i + k) mod n]`.

Every position of `t` is filled from the original `s` at once, not from
letters that have already been moved. Only places are counted: a letter is
never moved along the alphabet, so `t` is made of exactly the letters of `s`.

Return `t`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `1 <= k <= 10^4`
- `s` consists of lowercase English letters only.
