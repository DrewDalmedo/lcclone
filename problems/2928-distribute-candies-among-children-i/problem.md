id: 2928
slug: distribute-candies-among-children-i
title: Distribute Candies Among Children I
difficulty: easy
tags: math, combinatorics, enumeration
signature: int distributeCandies(int n, int limit)
---
Three children, the first, the second and the third, are to share `n`
identical candies. A way of sharing them says how many candies each child
gets: a triple `(a, b, c)` of whole numbers, for the first, second and third
child, such that

- every candy is given away: `a + b + c = n`;
- each child gets between `0` and `limit` candies: `0 <= a, b, c <= limit`.

A child may well get no candy at all. The children are told apart, so two
triples with the same numbers in another order, such as `(3, 0, 1)` and
`(1, 3, 0)`, are two different ways.

Return the number of different ways, or `0` if there is none.

@examples

## Constraints

- `1 <= n <= 50`
- `1 <= limit <= 50`
