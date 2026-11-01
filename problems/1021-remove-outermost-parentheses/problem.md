id: 1021
slug: remove-outermost-parentheses
title: Remove Outermost Parentheses
difficulty: easy
tags: stack, string
signature: string removeOuterParentheses(string s)
---
The string `s` is made of `(` and `)`. Walk through it from left to right
with a counter that starts at `0`, adding `1` at each `(` and subtracting
`1` at each `)`. In every `s` given here, the counter never goes below `0`
and is back at `0` at the end.

Each time the counter returns to `0`, a **block** of `s` ends, and the next
character starts a new block. So `s` is a row of blocks, one after another;
each block begins with a `(` and ends with the `)` that brings the counter
back to `0`, and in between the counter stays above `0`.

Delete the first and the last character of every block, keep all other
characters in their order, and return the resulting string. Only that one
outer pair goes from each block: the parentheses inside it stay, even where
they form pairs of their own. The result is `""` when every block is just
`()`.

@examples

## Constraints

- `1 <= s.length <= 10^5`
- `s[i]` is `(` or `)`.
- The counter described above never goes below `0` and ends at `0`.
