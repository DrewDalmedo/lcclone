id: 2869
slug: minimum-operations-to-collect-elements
title: Minimum Operations to Collect Elements
difficulty: easy
tags: bit-manipulation, array, hash-table
signature: int minOperations(int[] nums, int k)
---
`nums` describes a stack of numbered cards: `nums[0]` is the card at the
bottom, and the last element of `nums` is the card on top. In one move you
take the card that is on top at that moment and add it to your hand. Cards
can only be taken from the top, one per move.

Return the fewest moves after which your hand holds every number
`1, 2, ..., k`, each on at least one card.

A card numbered above `k`, or with a number already in your hand, still
costs a move but brings nothing new. Every number from `1` to `k` is on
some card, so taking the whole stack is always enough.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `1 <= nums[i] <= nums.length`
- `1 <= k <= nums.length`
- Each of the numbers `1, 2, ..., k` occurs in `nums` at least once.
