id: 824
slug: goat-latin
title: Goat Latin
difficulty: easy
tags: string
signature: string toGoatLatin(string sentence)
---
In Goat Latin, every word ends in a bleat, and the bleat grows by one `a`
from each word to the next. To translate `sentence`, number its words 1,
2, 3, ... from the left, and rewrite word number `i` in two steps:

1. Look at its first letter. If that letter is a vowel, keep the word as
   it is. If it is a consonant, take it off the front of the word and put
   it at the back, so `"Tree"` becomes `"reeT"` and `"w"` stays `"w"`.
2. Add the bleat of word `i` at the end: an `m` followed by `i + 1` letters
   `a`. The first word gets `maa`, the second `maaa`, the third `maaaa`.

A vowel here is one of `a`, `e`, `i`, `o` and `u`, in either case, so `E`
counts as much as `e`; all the other letters are consonants, and that
includes `y` and `Y`. Only the first letter of a word decides what step 1
does, and the rest of the word is never changed. No letter changes case
either: a capital sent to the back is still a capital there, and every
bleat is written in lowercase.

Return the rewritten words, in the order they had in `sentence` and joined
by single spaces.

@examples

## Constraints

- `1 <= sentence.length <= 150`
- `sentence` consists of English letters (`a`-`z`, `A`-`Z`) and spaces.
- There is no space at the start or the end of `sentence`, and exactly
  one space between neighbouring words.
