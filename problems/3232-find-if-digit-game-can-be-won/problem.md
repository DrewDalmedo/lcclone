id: 3232
slug: find-if-digit-game-can-be-won
title: Find if Digit Game Can Be Won
difficulty: easy
tags: array, math
signature: bool canAliceWin(int[] nums)
---
`nums` lists the numbers printed on a stack of cards, one number per card.
Every card is one of two kinds: a **short** card shows a number with one
digit (`1` to `9`), and a **long** card a number with two digits (`10` to
`99`). Equal numbers are separate cards.

Alice claims one kind, all of it: either every short card or every long
card. She can't leave out some cards of the kind she claims, or take cards
of both kinds. Bob then gets every card she didn't claim. A player's
**score** is the sum of the numbers on their cards, and Alice wins only by
beating Bob's score: equal scores are not a win.

Return `true` if Alice has a claim that wins, and `false` if neither claim
does. She may claim a kind that has no cards at all, for a score of `0`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 99`
