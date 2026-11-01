id: 2937
slug: make-three-strings-equal
title: Make Three Strings Equal
difficulty: easy
tags: string
signature: int findMinimumOperations(string s1, string s2, string s3)
---
You have three strings, `s1`, `s2` and `s3`, and you may trim them from
the end, one letter per move: each move takes the last letter off one
string of your choice. Every string must keep at least one letter, so a
string that is down to a single letter can't be trimmed any further.

Return the fewest moves after which the three strings read exactly the
same. If they already do, that is `0` moves. If no sequence of moves can
make them the same, return `-1`.

@examples

## Constraints

- `1 <= s1.length, s2.length, s3.length <= 100`
- `s1`, `s2` and `s3` consist of lowercase English letters only.
