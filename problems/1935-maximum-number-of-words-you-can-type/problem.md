id: 1935
slug: maximum-number-of-words-you-can-type
title: Maximum Number of Words You Can Type
difficulty: easy
tags: hash-table, string
signature: int canBeTypedWords(string text, string brokenLetters)
---
You want to copy out the line `text` on an old typewriter whose type bars
for some letters have snapped off. Striking one of the letters listed in
`brokenLetters` prints nothing; every other letter, and the space bar,
works as usual.

`text` is a sequence of words made of lowercase letters, with one space
between each word and the next. A word comes out **whole** if none of its
letters is listed in `brokenLetters`. A single broken letter anywhere in a
word, at its start, its end or in between, spoils that word, and a word
with several broken letters is still just one spoiled word.

Return how many words of `text` come out whole. Words are counted one by
one as they occur in `text`, so a whole word that appears twice adds `2`.
When `brokenLetters` is empty, nothing is broken and every word comes out
whole.

@examples

## Constraints

- `1 <= text.length <= 10^4`
- `0 <= brokenLetters.length <= 26`
- `text` consists of lowercase English letters and spaces. It does not
  start or end with a space, and never has two spaces in a row.
- `brokenLetters` consists of lowercase English letters, all different.
