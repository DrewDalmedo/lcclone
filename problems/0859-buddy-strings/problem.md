id: 859
slug: buddy-strings
title: Buddy Strings
difficulty: easy
tags: hash-table, string
signature: bool buddyStrings(string s, string goal)
---
`s` and `goal` are words made of lowercase letters. You must change `s` by
exactly one **swap**: pick two different positions of `s` and trade the
letters that stand there. The two letters may be the same, and then the
swap leaves `s` as it was. Skipping the swap is not allowed, and neither is
making a second one.

Return `true` if some single swap makes `s` read exactly like `goal`, and
`false` if none does. A swap never changes the length of `s`, so the answer
is `false` whenever the two words have different lengths.

@examples

## Constraints

- `1 <= s.length, goal.length <= 2 * 10^4`
- `s` and `goal` contain only lowercase English letters.
