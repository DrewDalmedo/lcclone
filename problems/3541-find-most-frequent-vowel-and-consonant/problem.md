id: 3541
slug: find-most-frequent-vowel-and-consonant
title: Find Most Frequent Vowel and Consonant
difficulty: easy
tags: hash-table, string, counting
signature: int maxFreqSum(string s)
---
Split the 26 lowercase letters into two teams. The vowel team has five
members, `a`, `e`, `i`, `o` and `u`; the consonant team has the other 21,
`y` among them.

Every letter scores one point for each position of the string `s` that
holds it. A team's top score is the highest score reached by any single
member of that team, or `0` when no member of the team appears in `s` at
all. Return the vowel team's top score plus the consonant team's top
score.

Only those two scores are added: the points of the other members don't
count, and when several members of one team share the top score it makes
no difference which of them you pick.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters only.
