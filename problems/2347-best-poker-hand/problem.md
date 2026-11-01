id: 2347
slug: best-poker-hand
title: Best Poker Hand
difficulty: easy
tags: array, hash-table, counting
signature: string bestHand(int[] ranks, char[] suits)
---
Your hand is five playing cards, described by two parallel arrays:
`ranks[i]`, a number from `1` to `13`, is the rank of card `i`, and
`suits[i]`, one of the letters `a`, `b`, `c` and `d`, is its suit. No two
of the cards have both the same rank and the same suit.

Go down this list of four labels, and give the hand the first one that fits
it:

1. `"Flush"`: the five suits are all the same letter.
2. `"Three of a Kind"`: some rank is on three or more of the cards.
3. `"Pair"`: some rank is on two or more of the cards.
4. `"High Card"`: any hand at all.

Return that label, spelled exactly as above, with the same capital letters
and single spaces.

These four are the only labels. Four cards of one rank, or three of one
rank and two of another, get `"Three of a Kind"`; two pairs of different
ranks get `"Pair"`; and ranks in a row, such as `4` to `8`, mean nothing
special.

@examples

## Constraints

- `ranks.length == suits.length == 5`
- `1 <= ranks[i] <= 13`
- `suits[i]` is `'a'`, `'b'`, `'c'` or `'d'`.
- No two cards have the same rank and the same suit.
