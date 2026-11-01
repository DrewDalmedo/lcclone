id: 1945
slug: sum-of-digits-of-string-after-convert
title: Sum of Digits of String After Convert
difficulty: easy
tags: string, simulation
signature: int getLucky(string s, int k)
---
Each lowercase letter has a **rank**, its place in the alphabet counted
from 1: `a` is 1, `b` is 2, and so on up to `z`, which is 26. Ranks are
written in decimal without leading zeros, so `e` gives the single digit
`5` and `t` the two digits `20`.

First, spell `s` out as a number: write down the rank of each letter of
`s`, in order, one right after another with nothing between them. For
`"tea"` that gives `2051`.

Then **fold** it `k` times. To fold, add up the decimal digits of the
number you hold; that total is the number you hold from then on. Spelling
`s` out is not a fold: the first fold acts on the spelled-out number, and
exactly `k` folds are made in all. Return the number you hold after the
last fold.

@examples

## Constraints

- `1 <= s.length <= 100`
- `1 <= k <= 10`
- `s` consists of lowercase English letters.
