id: 4043
slug: count-rotations-with-exactly-k-equal-adjacent-pairs
title: Count Rotations With Exactly K Equal Adjacent Pairs
difficulty: easy
tags: string, enumeration, sliding-window
signature: int countRotations(string s, int k)
---
Two letters standing side by side in a string make a **double** when they
are the same letter. `"committee"` has three doubles, `mm`, `tt` and `ee`.
A letter repeated `r` times in a row holds `r - 1` doubles, so `"zzzz"` has
three.

Let `n` be the length of `s`. For each `p` from `0` to `n - 1`, the
**shift by `p`** of `s` takes its first `p` letters off the front and puts
them, in the same order, at the back: it reads `s[p], s[p + 1], ...,
s[n - 1]` and then `s[0], s[1], ..., s[p - 1]`. The shift by `0` is `s`
itself, so `s` has exactly `n` shifts, one for each `p`.

The doubles of a shift are counted in the shifted string as it stands, from
its first letter to its last. Its last letter and its first letter are not
side by side, so they never make a double, even when they are the same.

Return how many of the `n` values of `p` give a shift with exactly `k`
doubles. Each `p` counts on its own, even when two values of `p` give the
same string: `"abab"` shifted by `0` and shifted by `2` is `"abab"` both
times, and that makes two shifts, not one.

@examples

## Constraints

- `2 <= s.length <= 100`
- `s` consists of lowercase English letters.
- `0 <= k <= s.length - 1`

## Follow-up

Building every shift and counting its doubles takes `O(n^2)` time. Can you
find the answer in `O(n)`, without building any shift?
