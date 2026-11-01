id: 1880
slug: check-if-word-equals-summation-of-two-words
title: Check if Word Equals Summation of Two Words
difficulty: easy
tags: string
signature: bool isSumEqual(string firstWord, string secondWord, string targetWord)
---
In a simple cipher, the ten letters `a` to `j` stand for the ten decimal
digits in order: `a` is 0, `b` is 1, and so on up to `j`, which is 9. A word
made of these letters encodes a whole number: swap each letter for its
digit and read the digits, left to right, as an ordinary base-10 number.
Zeros in front change nothing, so `"aadh"` encodes 37, and a word made only of
the letter `a` encodes 0.

Decode the three words. Return `true` if the number in `firstWord` plus the
number in `secondWord` equals the number in `targetWord`, and `false`
otherwise. Only the numbers matter, not how many letters each word uses.

@examples

## Constraints

- `1 <= firstWord.length, secondWord.length, targetWord.length <= 8`
- `firstWord`, `secondWord` and `targetWord` consist of the letters `a` to `j` only.
