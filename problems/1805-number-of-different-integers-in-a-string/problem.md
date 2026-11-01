id: 1805
slug: number-of-different-integers-in-a-string
title: Number of Different Integers in a String
difficulty: easy
tags: hash-table, string
signature: int numDifferentIntegers(string word)
---
In `word`, the letters act as walls between groups of digits. Call a
stretch of digit characters a **number** when no letter sits inside it and
it can't be stretched further: the characters just before and just after
it, where they exist, are letters.

A number stands for the non-negative integer it spells in decimal, and
leading zeros don't change that integer: `"0068"` stands for the same
integer as `"68"`, and `"0"` and `"0000"` both stand for zero. Zeros
anywhere else do count, so `"68"`, `"608"` and `"680"` are all different.
A number can be as long as `word` itself, far more digits than any built-in
integer type can hold.

Return how many different integers the numbers of `word` stand for. An
integer spelled by several numbers is counted once, and a `word` without
digits has no numbers at all, so the answer is then `0`.

@examples

## Constraints

- `1 <= word.length <= 1000`
- `word` consists of lowercase English letters and digits only.
