id: 2942
slug: find-words-containing-character
title: Find Words Containing Character
difficulty: easy
tags: array, string
signature: int[] findWordsContaining(string[] words, char x)
compare: unordered 0
---
`words` is a list of lowercase words and `x` is a single lowercase letter.
Find every word in which `x` appears at least once, and return the
positions of those words in the list (counting from `0`), *in any order*.
The same word may be listed more than once; each of its positions counts
on its own.

Return each position only once, even when `x` appears several times in
that word. If `x` appears in no word, return an empty array.

@examples

## Constraints

- `1 <= words.length <= 50`
- `1 <= words[i].length <= 50`
- `words[i]` consists of lowercase English letters only.
- `x` is a lowercase English letter.
