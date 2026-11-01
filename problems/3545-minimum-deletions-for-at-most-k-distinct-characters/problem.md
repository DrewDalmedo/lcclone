id: 3545
slug: minimum-deletions-for-at-most-k-distinct-characters
title: Minimum Deletions for At Most K Distinct Characters
difficulty: easy
tags: greedy, hash-table, string, counting, sorting
signature: int minDeletion(string s, int k)
---
A label printer holds at most `k` letter stamps, each for a different
letter, and you choose which ones to put in. You then print the string
`s` with it: every character of `s` whose stamp is in the printer comes
out, and every character whose stamp isn't is simply skipped.

Choose the stamps so that as few characters of `s` as possible are
skipped, and return how many are skipped then. Each skipped character
counts on its own, so a letter that occurs four times in `s` and has no
stamp costs `4`. When `s` uses `k` different letters or fewer, all of
their stamps fit and the answer is `0`.

@examples

## Constraints

- `1 <= s.length <= 16`
- `1 <= k <= 16`
- `s` consists of lowercase English letters only.
