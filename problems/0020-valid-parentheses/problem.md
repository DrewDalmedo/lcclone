id: 20
slug: valid-parentheses
title: Valid Parentheses
difficulty: easy
tags: string, stack
signature: bool isValid(string s)
---
The string `s` is made only of the six bracket characters `(`, `)`, `[`, `]`,
`{` and `}`. Read it from left to right while keeping track of the brackets
that are still open. Each closing bracket must be of the same kind as the
bracket opened most recently that is still open, and it closes that bracket.

The string is **balanced** when every closing bracket finds a match this way
and no bracket is left open at the end. Return `true` if `s` is balanced and
`false` otherwise.

@examples

## Constraints

- `1 <= s.length <= 10^4`
- `s` contains only the characters `()[]{}`.

## Follow-up

If `s` could only contain `(` and `)`, a single counter would be enough. Why
does that stop working once there are three kinds of brackets?
