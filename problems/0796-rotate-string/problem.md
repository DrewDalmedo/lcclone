id: 796
slug: rotate-string
title: Rotate String
difficulty: easy
tags: string, string-matching
signature: bool rotateString(string s, string goal)
---
Write the letters of `s` around a circle, in order and clockwise, so that
its last letter is followed by its first letter again. Pick any letter of
the circle as a starting point, and read clockwise from there until every
letter has been read once. What you read is a **rotation** of `s`.

Starting at `s[k]` gives `s[k]`, `s[k + 1]`, ..., up to the last letter of
`s`, and then `s[0]`, `s[1]`, ..., `s[k - 1]`. It is also what `s` becomes
when its first letter is moved to its end `k` times over. Starting at `s[0]`
gives `s` itself, so `s` is one of its own rotations.

Return `true` if `goal` is a rotation of `s`, and `false` if it isn't. A
rotation always has the length of `s`, so the answer is `false` whenever
`goal` is longer or shorter than `s`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `1 <= goal.length <= 100`
- `s` and `goal` consist of lowercase English letters.
