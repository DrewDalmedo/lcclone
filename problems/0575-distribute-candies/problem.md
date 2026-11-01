id: 575
slug: distribute-candies
title: Distribute Candies
difficulty: easy
tags: array, hash-table
signature: int distributeCandies(int[] candyType)
---
A jar holds `n` candies, where `n` is even, and `candyType[i]` labels the
kind of the candy numbered `i`: two candies are of the same kind exactly
when their labels are equal. The labels only tell the kinds apart; their
signs and sizes mean nothing else.

You may take exactly half of the jar, `n / 2` candies, chosen however you
like. Return the largest number of different kinds that the candies you
take can include.

@examples

## Constraints

- `n == candyType.length`
- `2 <= n <= 10^4`
- `n` is even.
- `-10^5 <= candyType[i] <= 10^5`
