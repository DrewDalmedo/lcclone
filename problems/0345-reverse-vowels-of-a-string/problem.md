id: 345
slug: reverse-vowels-of-a-string
title: Reverse Vowels of a String
difficulty: easy
tags: two-pointers, string
signature: string reverseVowels(string s)
---
Here the **vowels** are exactly ten characters: `a`, `e`, `i`, `o`, `u`
and their capitals `A`, `E`, `I`, `O`, `U`. Nothing else is a vowel, not
even `y` or `Y`.

Mark the positions of `s` that hold a vowel. Every other character stays
where it is. The vowels themselves are written back into the marked
positions in the opposite order: the first marked position gets the last
vowel of `s`, the second gets the second to last, and so on. Return the
string this gives.

A vowel keeps its case when it moves, and upper- and lowercase vowels
form one sequence. So if `s` starts with `E` and ends with `o`, the `o`
moves to the front and the `E` to the back, each in its own case.

@examples

## Constraints

- `1 <= s.length <= 3 * 10^5`
- `s` consists of printable ASCII characters (codes 32 to 126): letters,
  digits, spaces and punctuation.
