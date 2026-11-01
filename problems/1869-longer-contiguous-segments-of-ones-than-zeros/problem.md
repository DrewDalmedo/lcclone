id: 1869
slug: longer-contiguous-segments-of-ones-than-zeros
title: Longer Contiguous Segments of Ones than Zeros
difficulty: easy
tags: string
signature: bool checkZeroOnes(string s)
---
`s` is a string over the two characters `0` and `1`. A **block** is a
stretch of consecutive equal characters of `s` that cannot be extended:
the character just before it and the one just after it, where they exist,
are the other character, so a lone character is a block of length 1. In
`"0011101"`, the longest block of `1`s is `111` and the longest block of
`0`s is `00`.

Let `ones` be the length of the longest block of `1`s and `zeros` the length
of the longest block of `0`s. A character that never occurs in `s` has no
block at all, and its length counts as `0`. Return `true` if `ones > zeros`,
and `false` if `ones <= zeros`: a tie goes against the `1`s.

Only the single longest block of each character matters, not how many blocks
there are, nor how many `0`s and `1`s `s` holds in all.

@examples

## Constraints

- `1 <= s.length <= 100`
- Each character of `s` is `0` or `1`.
