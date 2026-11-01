id: 1668
slug: maximum-repeating-substring
title: Maximum Repeating Substring
difficulty: easy
tags: string, dynamic-programming, string-matching
signature: int maxRepeating(string sequence, string word)
---
Join `k` copies of `word`, each copy starting right where the previous
one ends: for `word = "mo"` and `k = 3` this gives `"momomo"`. Find the
largest `k` for which that string appears in `sequence` as a block of
consecutive letters, and return it. If `word` cannot be found in
`sequence` even once, return `0`.

So the answer is the length of the longest **chain** of copies of `word`
in `sequence`, where each copy begins at the letter just after the end of
the one before it. Some points to keep in mind:

- A chain may begin anywhere in `sequence`, including partway into another
  occurrence of `word`.
- Copies in a chain never share letters: inside `"aaaaa"`, a chain of
  `"aa"` has at most 2 copies (`"aaaa"`), not 4.
- Copies separated by even one other letter belong to different chains,
  and only the single best chain counts; chains are not added together.

@examples

## Constraints

- `1 <= sequence.length <= 100`
- `1 <= word.length <= 100`
- `sequence` and `word` consist of lowercase English letters.
