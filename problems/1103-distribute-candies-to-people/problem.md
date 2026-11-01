id: 1103
slug: distribute-candies-to-people
title: Distribute Candies to People
difficulty: easy
tags: math, simulation
signature: int[] distributeCandies(int candies, int num_people)
---
A bag holds `candies` sweets, and `num_people` friends sit around a table
in seats numbered `0` to `num_people - 1`. You hand the sweets out in
helpings, going round the table in seat order: helping `1` goes to seat
`0`, helping `2` to seat `1`, and so on, and after the last seat you carry
on with seat `0` again. In general, helping `g` goes to seat
`(g - 1) mod num_people`.

Helping `g` holds `g` sweets, so each helping is one sweet bigger than the
one before, and the sizes keep growing from one trip round the table to
the next. When the bag holds fewer sweets than the next helping should
hold, the friend whose turn it is gets all the sweets left, and you stop.
You also stop as soon as the bag is empty.

Return an array of `num_people` numbers: how many sweets each seat
received in all, in seat order. A seat that received nothing has `0`.

@examples

## Constraints

- `1 <= candies <= 10^9`
- `1 <= num_people <= 1000`
