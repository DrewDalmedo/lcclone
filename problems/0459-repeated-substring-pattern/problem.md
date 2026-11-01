id: 459
slug: repeated-substring-pattern
title: Repeated Substring Pattern
difficulty: easy
tags: string, string-matching
signature: bool repeatedSubstringPattern(string s)
---
Cut `s` into consecutive blocks that all have the same length: at least two
blocks, with no letters left over. Return `true` if some block length makes
every block the same string, and `false` if no block length does.

Put another way, `s` passes when it is a shorter string written out two or
more times back to back, with nothing between the copies and nothing after
the last one. Taking all of `s` as a single block doesn't count, so a
one-letter `s` gives `false`.

@examples

## Constraints

- `1 <= s.length <= 10^4`
- `s` consists of lowercase English letters.

## Follow-up

Trying each block length that divides `s.length` and comparing the blocks
is fast enough here. Can you decide it in time linear in `s.length`, however
many divisors it has?
