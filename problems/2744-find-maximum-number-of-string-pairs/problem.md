id: 2744
slug: find-maximum-number-of-string-pairs
title: Find Maximum Number of String Pairs
difficulty: easy
tags: array, hash-table, string, simulation
signature: int maximumNumberOfStringPairs(string[] words)
---
Every entry of `words` is a code of exactly two lowercase letters, and no
code occurs twice in the list. Two entries at different positions are
**mirrors** when swapping the two letters of one gives the other: `"kt"`
and `"tk"` are mirrors, while `"kt"` and `"tm"`, which have only one
letter in common, are not.

You want to match up mirrors two by two, using each entry in at most one
match. Return the largest number of matches you can make.

Because the codes are all different, an entry has at most one mirror in
the list, so matches never compete for an entry and every pair of mirrors
can be matched. A code whose two letters are the same, such as `"ee"`, is
unchanged by the swap; since the list holds no second `"ee"`, it never has
a mirror.

@examples

## Constraints

- `1 <= words.length <= 50`
- `words[i].length == 2`
- `words[i]` consists of lowercase English letters.
- The codes in `words` are all different.
