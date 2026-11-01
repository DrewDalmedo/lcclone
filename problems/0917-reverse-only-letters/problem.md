id: 917
slug: reverse-only-letters
title: Reverse Only Letters
difficulty: easy
tags: two-pointers, string
signature: string reverseOnlyLetters(string s)
---
In this problem only the 52 English letters, `a` to `z` and `A` to `Z`,
count as **letters**. Digits and symbols such as `@`, `[`, `_` or the
backtick are not letters, even those whose codes lie between `Z` and `a`.

Return a string of the same length as `s` that obeys two rules:

- each character that is not a letter is pinned: it keeps its index;
- the letters are spread over the indices that are left, in such a way
  that the letters of the result, read from left to right, spell the
  letters of `s` read from right to left.

Case belongs to the letter, not to the index: an uppercase letter that
moves to an index where a lowercase one stood is still uppercase in the
result, and the other way round.

@examples

## Constraints

- `1 <= s.length <= 100`
- Every character of `s` has an ASCII code from 33 to 122 (`!` to `z`).
- `s` contains no `"` and no `\`.
