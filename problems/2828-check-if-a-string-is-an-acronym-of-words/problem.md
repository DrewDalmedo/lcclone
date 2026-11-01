id: 2828
slug: check-if-a-string-is-an-acronym-of-words
title: Check if a String Is an Acronym of Words
difficulty: easy
tags: array, string
signature: bool isAcronym(string[] words, string s)
---
Go through the list `words` from its first entry to its last and note down
the opening letter of each word, ignoring the rest of it. The letters you
noted, kept in that order, spell the **initials** of the list: a string
with exactly one letter per word, so it has as many letters as `words` has
entries.

Return `true` if `s` is exactly the initials of `words`, and `false`
otherwise.

Every word contributes its letter once, in its own place: `s` may not skip
a word, put the letters in another order, or add letters of its own. So an
`s` that stops partway through the initials, or that goes on after them,
gives `false`.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 10`
- `1 <= s.length <= 100`
- `words[i]` and `s` consist of lowercase English letters.
