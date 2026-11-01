id: 830
slug: positions-of-large-groups
title: Positions of Large Groups
difficulty: easy
tags: string
signature: int[][] largeGroupPositions(string s)
---
On a keyboard with sticky keys, a single press can come out as the same
letter several times over. To find the spots worth a second look in a text
`s` typed on one, divide `s` into **groups**, going from left to right: the
first letter opens a group, and each later letter joins the current group
when it equals the letter before it, or opens a new group when it doesn't.
So `"zooom"` has the groups `"z"`, `"ooo"` and `"m"`, and `"aabaa"` has
three groups: its two `"aa"`s are separate, with the `b` between them.

A group is **large** when it is at least 3 letters long. Report each large
group as the pair `[start, end]`, where `start` is the index of its first
letter and `end` the index of its last. Return these pairs in the order
their groups appear in `s`, so that `start` increases, or an empty array
when `s` has no large group.

@examples

## Constraints

- `1 <= s.length <= 1000`
- `s` consists of lowercase English letters.
