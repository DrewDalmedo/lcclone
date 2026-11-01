id: 3120
slug: count-the-number-of-special-characters-i
title: Count the Number of Special Characters I
difficulty: easy
tags: hash-table, string
signature: int numberOfSpecialChars(string word)
---
Every character of `word` is an English letter, typed either in uppercase
(`A` to `Z`) or in lowercase (`a` to `z`). Go through the 26 letters of the
alphabet and pick out each letter for which `word` holds at least one
uppercase copy **and** at least one lowercase copy. Return how many letters
you picked.

Each letter is picked once or not at all: extra copies of either form add
nothing. The positions of the copies don't matter either, so the uppercase
form may come before or after the lowercase one. In `"mMm"` and in
`"MMm"`, the letter m is picked, and the answer is 1.

@examples

## Constraints

- `1 <= word.length <= 50`
- `word` consists of English letters only, `a` to `z` and `A` to `Z`.
