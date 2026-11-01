id: 2839
slug: check-if-strings-can-be-made-equal-with-operations-i
title: Check if Strings Can be Made Equal With Operations I
difficulty: easy
tags: string
signature: bool canBeEqual(string s1, string s2)
---
Picture each of `s1` and `s2` as a row of four letter cards, at positions
`0`, `1`, `2` and `3`. A **jump** takes two cards of one row that have
exactly one card between them, at positions `i` and `i + 2`, and puts each
of them where the other one was. A row of four allows just two jumps: one
between positions `0` and `2`, and one between positions `1` and `3`.

You may make as many jumps as you like, one after another, in either row
or in both; making none at all is allowed too. Return `true` if the two
rows can be brought to spell exactly the same thing, and `false` if no
sequence of jumps does it.

Letters may repeat, both within a row and between the two rows.

@examples

## Constraints

- `s1.length == s2.length == 4`
- `s1` and `s2` consist of lowercase English letters.
