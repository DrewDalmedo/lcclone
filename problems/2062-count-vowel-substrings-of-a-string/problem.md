id: 2062
slug: count-vowel-substrings-of-a-string
title: Count Vowel Substrings of a String
difficulty: easy
tags: hash-table, string
signature: int countVowelSubstrings(string word)
---
Choose a start and an end in `word` and highlight every letter from the
one to the other, both included, so that a highlight is one or more
consecutive letters. Call a highlight **good** when both of these hold:

- every letter it covers is one of the vowels `a`, `e`, `i`, `o` and `u`
  (`y` counts as a consonant, like every other letter), and
- each of the five vowels is covered at least once; a vowel may also be
  covered several times.

Return the number of good highlights in `word`. Highlights that start or
end at different places count separately, even when the letters they cover
spell the same thing.

@examples

## Constraints

- `1 <= word.length <= 100`
- `word` consists of lowercase English letters.
