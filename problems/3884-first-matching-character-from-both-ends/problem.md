id: 3884
slug: first-matching-character-from-both-ends
title: First Matching Character From Both Ends
difficulty: easy
tags: two-pointers, string
signature: int firstMatchingIndex(string s)
---
Write the string `s`, of length `n`, on a strip of paper, one letter per
square, and fold the strip in the middle so that its two ends meet. Each
square `i` (counting from `0`) then lies on top of square `n - 1 - i`:
square `0` lands on square `n - 1`, square `1` on square `n - 2`, and so
on toward the fold. When `n` is odd, the middle square has no other square to land on
and lies on itself, so it always counts as matching.

Return the smallest index `i` such that `s[i]` and `s[n - 1 - i]` are the
same letter. If no square matches the one it lies on, return `-1`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters only.
