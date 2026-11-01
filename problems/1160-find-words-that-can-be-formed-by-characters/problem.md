id: 1160
slug: find-words-that-can-be-formed-by-characters
title: Find Words That Can Be Formed by Characters
difficulty: easy
tags: array, hash-table, string, counting
signature: int countCharacters(string[] words, string chars)
---
A printer keeps a case of metal type, and `chars` lists its pieces: one
letter per piece, so a letter that appears three times in `chars` has three
pieces. To print a word, the printer sets one piece in place for each
letter of the word, and a piece can stand in only one place. So a word can
be printed exactly when no letter occurs in it more times than in `chars`.
Pieces the word doesn't need simply stay in the case, and the order of the
letters in `chars` plays no part.

The printer tries each entry of `words` on its own, with the full case:
once a word has been tried, all its pieces go back into the case before
the next word.

Return the total length of the entries of `words` that can be printed. A
word listed more than once counts every time it is listed. If no entry can
be printed, return `0`.

@examples

## Constraints

- `1 <= words.length <= 1000`
- `1 <= words[i].length <= 100`
- `1 <= chars.length <= 100`
- `words[i]` and `chars` consist of lowercase English letters.
