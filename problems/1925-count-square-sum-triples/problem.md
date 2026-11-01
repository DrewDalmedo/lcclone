id: 1925
slug: count-square-sum-triples
title: Count Square Sum Triples
difficulty: easy
tags: math, enumeration
signature: int countTriples(int n)
---
Pick three positive whole numbers. If the square of the third one comes
out exactly equal to the squares of the first two added together, the
three, in that order, form a **square sum triple** `(a, b, c)` with
`a^2 + b^2 = c^2`. They are then the side lengths of a right triangle,
with `c` the side facing the right angle.

Count the square sum triples in which none of `a`, `b` and `c` goes above
`n`, and return the count.

The first two places can be swapped: `(5, 12, 13)` and `(12, 5, 13)` are
two different triples, and both count. The third place cannot: it always
holds the number whose square is the sum. So every right triangle with
whole-number sides gives exactly two triples.

Seen on a grid, you are counting the points `(a, b)` with
`1 <= a, b <= n` whose distance from `(0, 0)` is a whole number that is
at most `n`.

@examples

## Constraints

- `1 <= n <= 250`
