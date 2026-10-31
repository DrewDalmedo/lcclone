id: 171
slug: excel-sheet-column-number
title: Excel Sheet Column Number
difficulty: easy
tags: math, string
signature: int titleToNumber(string columnTitle)
---
The columns of a spreadsheet are numbered 1, 2, 3, ... from the left, and
each one also carries a label of capital letters. Column 1 is labelled
`A`. Every later column gets the label of the column just before it,
stepped forward like this:

- find the rightmost letter that is not `Z`, move it to the next letter of
  the alphabet, and turn every letter after it (all of them `Z`s) into `A`;
- if every letter is `Z`, the new label is one letter longer and made only
  of `A`s.

So `Z` is followed by `AA`, `CZ` by `DA`, and `ZZ` by `AAA`.

`columnTitle` is the label of some column. Return that column's number.

You can also compute the number directly. Give each letter its place in
the alphabet, from 1 for `A` to 26 for `Z`. Start from 0 and go through the
label from left to right: at each letter, multiply what you have by 26 and
add the letter's place. What you end with is the column number.

@examples

## Constraints

- `1 <= columnTitle.length <= 7`
- `columnTitle` consists of uppercase English letters.
- `columnTitle` is the label of a column numbered at most `2^31 - 1` (that
  column is `"FXSHRXW"`), so the answer fits in an `int`.
