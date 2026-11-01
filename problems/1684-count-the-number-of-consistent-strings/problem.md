id: 1684
slug: count-the-number-of-consistent-strings
title: Count the Number of Consistent Strings
difficulty: easy
tags: bit-manipulation, array, hash-table, string, counting
signature: int countConsistentStrings(string allowed, string[] words)
---
A sign painter owns one stencil for each letter of `allowed`, and no
others. The letters of `allowed` are all different, and a stencil can be
used as many times as needed, so a painted word may repeat a letter.

Call a word **consistent** when the painter can paint it with these
stencils alone: each of its letters, wherever it stands in the word, is
one of the letters of `allowed`. The word need not use every stencil, but
a single letter that is not in `allowed` rules it out.

Return how many entries of `words` are consistent. A word that occurs
several times in `words` counts once for each time it occurs.

@examples

## Constraints

- `1 <= words.length <= 10^4`
- `1 <= allowed.length <= 26`
- `1 <= words[i].length <= 10`
- The letters of `allowed` are pairwise distinct.
- `allowed` and every `words[i]` consist of lowercase English letters.
