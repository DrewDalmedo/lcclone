id: 3136
slug: valid-word
title: Valid Word
difficulty: easy
tags: string
signature: bool isValid(string word)
---
Each character of `word` is an English letter (lowercase or capital), a
digit from `0` to `9`, or a symbol. The letters fall into two classes: the
**vowels** are `a`, `e`, `i`, `o`, `u` and their capitals `A`, `E`, `I`,
`O`, `U`, and the **consonants** are all the other letters, `y` and `Y`
among them. Digits and symbols belong to neither class: a digit is not a
vowel, and it is not a consonant either.

Return `true` if `word` meets all four of these requirements, and `false`
if it misses even one:

- it is 3 or more characters long, every character counting, digits
  included;
- it is made of letters and digits alone, with no symbol anywhere in it;
- at least one of its characters is a vowel;
- at least one of its characters is a consonant.

Only which classes turn up matters, not where they stand: the vowel may come
before or after the consonants, and digits may sit anywhere.

@examples

## Constraints

- `1 <= word.length <= 20`
- `word` consists of English letters (lowercase and capital), digits, and
  the symbols `'@'`, `'#'` and `'$'`.
