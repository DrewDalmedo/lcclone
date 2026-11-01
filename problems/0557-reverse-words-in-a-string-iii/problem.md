id: 557
slug: reverse-words-in-a-string-iii
title: Reverse Words in a String III
difficulty: easy
tags: two-pointers, string
signature: string reverseWords(string s)
---
`s` is made of one or more words, with exactly one space between each word
and the next and no space at either end; if `s` holds no space at all, the
whole of it is a single word. Any printable character other than the space
can be part of a word: letters of either case, digits, punctuation, `"`
and `\` alike.

Spell each word backwards where it stands, and return the string that
results. The words stay in the same order, every space keeps its index, and
a letter keeps its case when it moves.

@examples

## Constraints

- `1 <= s.length <= 5 * 10^4`
- `s` consists of printable ASCII characters (codes 32 to 126).
- `s` does not begin or end with a space, and no two spaces in `s` are next
  to each other.
- `s` contains at least one word.
