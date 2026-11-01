id: 2287
slug: rearrange-characters-to-make-target-string
title: Rearrange Characters to Make Target String
difficulty: easy
tags: hash-table, string, counting
signature: int rearrangeCharacters(string s, string target)
---
A **spelling** of `target` is a set of positions of `s` whose characters
can be arranged to read exactly `target`: for every letter, the set has
as many positions holding that letter as `target` has occurrences of it.
The positions may lie anywhere in `s`, together or scattered, and in any
order.

Return the largest number of spellings of `target` that can be picked at
the same time, with no position of `s` belonging to two of them.
Positions that no spelling uses are allowed. If `s` can't provide even
one spelling, return `0`.

@examples

## Constraints

- `1 <= s.length <= 100`
- `1 <= target.length <= 10`
- `s` and `target` consist of lowercase English letters.
