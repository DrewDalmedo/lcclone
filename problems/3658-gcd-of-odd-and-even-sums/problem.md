id: 3658
slug: gcd-of-odd-and-even-sums
title: GCD of Odd and Even Sums
difficulty: easy
tags: math, number-theory
signature: int gcdOfOddEvenSums(int n)
---
Make a table with `n` rows and two columns. Row `i`, counting rows from
`1`, holds the `i`-th odd positive integer, `2i - 1`, in its left column
and the `i`-th even positive integer, `2i`, in its right column. So the
left column reads `1, 3, 5, ...` from the top and the right column
`2, 4, 6, ...`; zero is not positive, so it is never in the table.

Add up each column. Return the greatest common divisor of the two
totals: the largest positive integer that divides both of them with no
remainder.

@examples

## Constraints

- `1 <= n <= 1000`
