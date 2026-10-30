id: 14
slug: longest-common-prefix
title: Longest Common Prefix
difficulty: easy
tags: string, trie
signature: string longestCommonPrefix(string[] strs)
---
You are given a list of words `strs`. Return the longest string that every
word in the list begins with. If the words don't all begin with the same
letter, that string is empty, so return `""`.

A word counts as beginning with itself, so for a single word the answer is
the whole word.

@examples

## Constraints

- `1 <= strs.length <= 200`
- `0 <= strs[i].length <= 200`
- `strs[i]` consists of lowercase English letters only.
