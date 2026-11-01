id: 1859
slug: sorting-the-sentence
title: Sorting the Sentence
difficulty: easy
tags: string, sorting
signature: string sortSentence(string s)
---
Someone took a sentence of one to nine words, each word made of one or more
English letters of either case (one word may mix the two), and scrambled it
in two steps. First every word was tagged with its place in the sentence: one
digit, counting from `1`, written right after the word's last letter. Then
the tagged words were shuffled into some order, possibly the original one,
and written out with a single space between neighbours.

`s` is the result. Undo the scrambling and return the original sentence:
the words back in the order of their digits, the digits removed, a single
space between neighbouring words and none at the start or the end. The
letters of every word stay exactly as they are in `s`, case included.

@examples

## Constraints

- `2 <= s.length <= 200`
- `s` consists of English letters (`a`-`z`, `A`-`Z`), spaces and the digits
  `1` to `9`.
- `s` holds from `1` to `9` words, separated by single spaces, with no space
  at the start or the end.
- `s` was scrambled as described above: each of its `k` words is one or
  more letters followed by a single digit, and those digits are `1` to `k`,
  each once.
