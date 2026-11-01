id: 953
slug: verifying-an-alien-dictionary
title: Verifying an Alien Dictionary
difficulty: easy
tags: array, hash-table, string
signature: bool isAlienSorted(string[] words, string order)
---
The string `order` is a reshuffled alphabet. It lists each of the 26
lowercase letters exactly once, and one letter ranks before another when it
appears earlier in `order`: `order[0]` ranks first and `order[25]` last.

Words are compared as a dictionary would compare them, but with this
alphabet. Look for the first position at which the two words have
different letters: the word whose letter there comes earlier in `order` is
the smaller one, whatever comes after. If the words agree all the way to
the end of the shorter one, the shorter word is the smaller (as `"car"`
comes before `"cart"`), and two identical words are equal.

Return `true` if every word of `words` is smaller than or equal to the word
right after it, and `false` otherwise. A list of a single word is always in
order.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 20`
- `order.length == 26`
- Every letter from `'a'` to `'z'` occurs exactly once in `order`.
- `words[i]` consists of lowercase English letters.
