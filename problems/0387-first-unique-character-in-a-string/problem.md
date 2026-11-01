id: 387
slug: first-unique-character-in-a-string
title: First Unique Character in a String
difficulty: easy
tags: queue, hash-table, string, counting
signature: int firstUniqChar(string s)
---
Call a letter of `s` **lonely** if it occurs exactly once in the whole
string. Where the other copies of a letter stand doesn't matter: a letter
that turns up a second time anywhere in `s`, right beside the first or far
away from it, is not lonely.

Positions in `s` are counted from 0. Return the position of the leftmost
lonely letter. If `s` has none, because every letter in it occurs two or
more times, return `-1`.

The answer is a position, not a letter. When `s` has several lonely
letters, the one that stands earliest in `s` wins, whatever their order in
the alphabet.

@examples

## Constraints

- `1 <= s.length <= 10^5`
- `s` consists of lowercase English letters.

## Follow-up

Comparing each letter with every other one takes `O(n^2)` time. Can you
find the answer in `O(n)` time, with only a fixed amount of extra memory?
