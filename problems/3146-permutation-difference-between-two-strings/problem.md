id: 3146
slug: permutation-difference-between-two-strings
title: Permutation Difference between Two Strings
difficulty: easy
tags: hash-table, string
signature: int findPermutationDifference(string s, string t)
---
Think of each letter of `s` as a runner wearing that letter on their shirt.
`s` lists the runners in the order they lined up at the start of a race, and
`t` lists the same runners in the order they crossed the finish line. Places
are counted from `0`: the runner at position `i` of `s` started in place
`i`, and the runner at position `j` of `t` finished in place `j`.

A runner who started in place `i` and finished in place `j` moved
`|i - j|` places, whether they gained or lost ground. Return the total
number of places moved by all the runners together.

No letter occurs twice in `s`, and `t` holds exactly the letters of `s`,
rearranged, so every runner has one starting place and one finishing place.

@examples

## Constraints

- `1 <= s.length <= 26`
- `t.length == s.length`
- `s` consists of lowercase English letters, and no letter occurs in it
  more than once.
- `t` is a rearrangement of `s`.
