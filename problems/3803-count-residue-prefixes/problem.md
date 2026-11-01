id: 3803
slug: count-residue-prefixes
title: Count Residue Prefixes
difficulty: easy
tags: hash-table, string
signature: int residuePrefixes(string s)
---
Read the string `s` one letter at a time, from the left. Each time you
have just read a letter, look at two numbers:

- `r`, the remainder when the number of letters read so far is divided by
  `3` (so `r` is `0`, `1` or `2`);
- `k`, how many different letters have turned up so far. A letter that
  has been read several times counts once.

The moment **scores** when `k` equals `r`. Take `k` as it is, not its
remainder: with 4 letters read and 4 different ones among them, `r` is
`1`, `k` is `4`, and the moment doesn't score.

Return how many of the `s.length` moments score. Each moment stands for
the prefix of `s` read up to then, from one letter up to the whole string;
the moment before the first letter is read is not one of them.

@examples

## Constraints

- `1 <= s.length <= 100`
- `s` consists of lowercase English letters.
