id: 1002
slug: find-common-characters
title: Find Common Characters
difficulty: easy
tags: array, hash-table, string
signature: string[] commonChars(string[] words)
compare: unordered 0
---
You are given a list of lowercase words, `words`. A letter is **common** to
all of them as many times as it occurs in the word where it occurs the
fewest times. For example, a letter that occurs twice in one word, three
times in another and twice in a third is common twice. A letter missing
from even one word is not common at all.

Return the common letters, each as a string of one character, with every
letter repeated as many times as it is common. The strings may be returned
*in any order*. If no letter occurs in every word, return an empty array.

@examples

## Constraints

- `1 <= words.length <= 100`
- `1 <= words[i].length <= 100`
- `words[i]` consists of lowercase English letters.
