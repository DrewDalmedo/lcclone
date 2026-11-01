id: 1528
slug: shuffle-string
title: Shuffle String
difficulty: easy
tags: array, string
signature: string restoreString(string s, int[] indices)
---
The `n` letters of `s` have to be moved around. `indices` has one entry per
letter, and `indices[i]` is the place where the letter `s[i]` must end up:
in the string you build, position `indices[i]` holds `s[i]`. Positions are
counted from `0`.

Each of the positions `0` to `n - 1` appears exactly once in `indices`, so
every position of the new string gets exactly one letter. Return the new
string.

Mind the direction: `indices[i]` names where `s[i]` goes, not where the
letter for position `i` comes from.

@examples

## Constraints

- `n == s.length == indices.length`
- `1 <= n <= 100`
- `s` consists of lowercase English letters only.
- `0 <= indices[i] < n`
- The values of `indices` are all different.
