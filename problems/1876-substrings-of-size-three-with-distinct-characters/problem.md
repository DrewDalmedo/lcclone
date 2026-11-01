id: 1876
slug: substrings-of-size-three-with-distinct-characters
title: Substrings of Size Three with Distinct Characters
difficulty: easy
tags: hash-table, string, counting, sliding-window
signature: int countGoodSubstrings(string s)
---
`s` is a string of lowercase letters. For every index `i` with
`i + 2 < s.length`, the three consecutive characters `s[i]`, `s[i + 1]` and
`s[i + 2]` form the **triple** that starts at `i`. A triple is **good** when
its three letters are pairwise different: no two of them match, and that
includes the first and the third, not only letters side by side.

Return the number of good triples in `s`. Triples are told apart by where
they start, so a good triple whose letters turn up again at another start
counts again there. When `s` has fewer than three characters it has no
triples, and the answer is `0`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters.
