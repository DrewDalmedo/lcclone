id: 2042
slug: check-if-numbers-are-ascending-in-a-sentence
title: Check if Numbers Are Ascending in a Sentence
difficulty: easy
tags: string
signature: bool areNumbersAscending(string s)
---
`s` is made of **pieces** separated by single spaces. Each piece is either
a **word**, written with lowercase letters only, or a **number**, written
with digits only: a positive integer in ordinary decimal form, with no
leading zeros.

Ignore the words and pick out the numbers, keeping the order in which they
stand in `s`. Return `true` if each number after the first is larger than the
one listed just before it, and `false` as soon as some number is equal to
or smaller than the one before it.

Numbers are compared by their value, not as text, so `9` comes before `10`.
Words standing between two numbers change nothing: those two numbers are
still next to each other on the list.

@examples

## Constraints

- `3 <= s.length <= 200`
- `s` consists of lowercase English letters, digits and spaces.
- `s` has from `2` to `100` pieces, with exactly one space between
  neighbouring pieces and no space at the start or end.
- At least two of the pieces are numbers.
- Every number is below `100` and has no leading zeros.
