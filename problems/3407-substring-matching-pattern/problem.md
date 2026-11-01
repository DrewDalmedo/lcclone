id: 3407
slug: substring-matching-pattern
title: Substring Matching Pattern
difficulty: easy
tags: string, string-matching
signature: bool hasMatch(string s, string p)
---
You are searching the text `s` with the pattern `p`. The pattern is made of
lowercase letters plus exactly one `*`, a wildcard that may be filled in with
any run of letters: one letter, many, or none at all. Every letter of `p`
must be matched exactly as written.

Return `true` if the `*` can be filled in so that the resulting word occurs
in `s` as a block of adjacent letters, and `false` if no filling works.

Another way to see it: the `*` cuts `p` into a left part and a right part,
either of which may be empty. `s` matches when it contains the left part and,
starting no earlier than where that copy of the left part ends, the right
part. The two copies may sit right next to each other (the `*` is then
empty), but they may not share any letter. Each part's letters have to be
adjacent in `s`; only the `*` may skip over letters.

@examples

## Constraints

- `1 <= s.length <= 50`
- `1 <= p.length <= 50`
- `s` consists of lowercase English letters.
- `p` consists of lowercase English letters and exactly one `*`.
