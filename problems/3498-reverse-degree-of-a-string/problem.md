id: 3498
slug: reverse-degree-of-a-string
title: Reverse Degree of a String
difficulty: easy
tags: string, simulation
signature: int reverseDegree(string s)
---
Number the letters of the alphabet backwards: `z` gets `1`, `y` gets `2`,
and so on, until `a` gets `26`. Call that number a letter's **weight**.

Let `n` be the length of `s`. Its characters stand at places `1`, `2`, ...,
`n` from left to right, so the first character is at place `1`, not `0`.
Each character contributes its weight times its place, and the **reverse
degree** of `s` is the total of all these contributions. Return it.

A letter that appears several times contributes once for each place it
stands at.

@examples

## Constraints

- `1 <= s.length <= 1000`
- `s` consists of lowercase English letters only.
