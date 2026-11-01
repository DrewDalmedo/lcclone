id: 2586
slug: count-the-number-of-vowel-strings-in-range
title: Count the Number of Vowel Strings in Range
difficulty: easy
tags: array, string, counting
signature: int vowelStrings(string[] words, int left, int right)
---
Here the vowels are the five letters `a`, `e`, `i`, `o` and `u`; every
other letter, `y` included, is a consonant. Call a word **vowel-capped**
when its first letter and its last letter are both vowels. The letters in
between don't matter, and a word of one letter is vowel-capped exactly when
that letter is a vowel.

The positions in `words` are numbered from `0`. Look only at the entries
from position `left` up to position `right`, both included, and return how
many of them are vowel-capped. A word that appears at several of those
positions counts once for each.

@examples

## Constraints

- `1 <= words.length <= 1000`
- `1 <= words[i].length <= 10`
- `words[i]` consists of lowercase English letters only.
- `0 <= left <= right < words.length`
