id: 3304
slug: find-the-k-th-character-in-string-game-i
title: Find the K-th Character in String Game I
difficulty: easy
tags: bit-manipulation, recursion, math, simulation
signature: char kthCharacter(int k)
---
A word is built up in rounds. It starts as the single letter `"a"`. In
each round, make a copy of the whole word in which every letter moves one
place on in the alphabet (`a` becomes `b`, `b` becomes `c`, and so on,
with `z` going back round to `a`), and attach that copy to the end of the
word. So every round doubles the length, and the letters already there
never change: after two rounds the word is `"abbc"`, and later rounds only
add letters after those four.

Positions in the word are counted from 1. Return the letter at position
`k`. It is settled from the moment the word first reaches `k` letters, and
no later round changes it.

@examples

## Constraints

- `1 <= k <= 500`
