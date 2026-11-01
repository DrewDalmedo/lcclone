id: 1941
slug: check-if-all-characters-have-equal-number-of-occurrences
title: Check if All Characters Have Equal Number of Occurrences
difficulty: easy
tags: hash-table, string, counting
signature: bool areOccurrencesEqual(string s)
---
Read `s` as a pile of ballots: each character is one vote, cast for the
candidate named by that letter. A **candidate** is a letter that received
at least one vote, that is, a letter occurring somewhere in `s`.

Return `true` if the election is a perfect tie, with every candidate
receiving exactly as many votes as every other candidate, and `false` if
some candidate received more votes than another.

Letters that occur nowhere in `s` are not candidates, so they never break
the tie. When only one letter occurs in `s`, that candidate is tied with
itself and the answer is `true`.

@examples

## Constraints

- `1 <= s.length <= 1000`
- `s` consists of lowercase English letters.
