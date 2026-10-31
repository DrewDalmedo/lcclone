id: 58
slug: length-of-last-word
title: Length of Last Word
difficulty: easy
tags: string
signature: int lengthOfLastWord(string s)
---
The string `s` is a line of text made of letters and spaces. Cut it into
**words** at the spaces: a word is a run of letters that is as long as it
can be, so it has a space or an end of `s` on each side of it.

Return the number of letters in the word nearest the end of `s`.

The spaces are not tidy. There may be any number of them before the first
word, between two words and after the last word, and none of them belong
to a word. Every letter counts as one, whether upper or lower case.

@examples

## Constraints

- `1 <= s.length <= 10^4`
- `s` consists of English letters (`a`-`z`, `A`-`Z`) and spaces `' '`.
- `s` contains at least one word.
