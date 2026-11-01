id: 500
slug: keyboard-row
title: Keyboard Row
difficulty: easy
tags: array, hash-table, string
signature: string[] findWords(string[] words)
---
The 26 letter keys of a US QWERTY keyboard are laid out in three rows:

- the top row holds `q w e r t y u i o p`;
- the middle row holds `a s d f g h j k l`;
- the bottom row holds `z x c v b n m`.

Each letter is on exactly one of these rows, and a capital letter is typed
with the same key as its lowercase form: `K` and `k` are both on the middle
row. No other key matters here.

Go through `words` and keep each word whose letters all sit on one and
the same row; a word that needs keys from two or three rows is dropped.
Return the kept words in the order they come in `words`, each spelled
exactly as it is there, capitals included. Repeats are kept as often as
they occur, and words that differ only in case count as different words.
If every word needs more than one row, return an empty array.

@examples

## Constraints

- `1 <= words.length <= 20`
- `1 <= words[i].length <= 100`
- `words[i]` consists of lowercase and uppercase English letters.
