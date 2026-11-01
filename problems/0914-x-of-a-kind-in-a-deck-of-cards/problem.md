id: 914
slug: x-of-a-kind-in-a-deck-of-cards
title: X of a Kind in a Deck of Cards
difficulty: easy
tags: array, hash-table, math, counting, number-theory
signature: bool hasGroupsSizeX(int[] deck)
---
Every card in a deck has a number printed on it: card `i` shows `deck[i]`.

Pick a pile size `X` of at least `2`, then try to deal out the **whole**
deck into piles of exactly `X` cards, every card going into exactly one
pile, with no pile mixing two different numbers. Two or more piles may
show the same number, and the order of the cards in `deck` doesn't matter.

Return `true` if some choice of `X` makes such a deal possible, and
`false` if none does.

@examples

## Constraints

- `1 <= deck.length <= 10^4`
- `0 <= deck[i] < 10^4`
