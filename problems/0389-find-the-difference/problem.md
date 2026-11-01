id: 389
slug: find-the-difference
title: Find the Difference
difficulty: easy
tags: bit-manipulation, hash-table, string, sorting
signature: char findTheDifference(string s, string t)
---
Write each character of `s` on a card of its own. One more card, with a
lowercase letter on it, joins them, and then all the cards are dealt out
in a row in any order, perhaps even the order they started in. Reading
the row from left to right gives `t`. Return the letter on the card that
joined.

The positions in `t` can't be relied on; the counts can. `t` has every
letter exactly as many times as `s` has it, except for one letter, which
it has once more: that letter is the answer. It may well be a letter that
`s` already contains, so it can't always be spotted as a letter of `t`
that `s` lacks.

@examples

## Constraints

- `0 <= s.length <= 1000`
- `t.length == s.length + 1`
- `s` and `t` consist of lowercase English letters.
- `t` is made from `s` as described: the letters of `s` and one more,
  in any order.
