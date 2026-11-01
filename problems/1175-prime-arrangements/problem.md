id: 1175
slug: prime-arrangements
title: Prime Arrangements
difficulty: easy
tags: math
signature: int numPrimeArrangements(int n)
---
You have `n` cards, numbered `1` to `n`, and a row of `n` slots, also
numbered `1` to `n` from left to right. A **layout** puts one card in each
slot. A layout is **valid** when every card with a prime number lies in a
slot with a prime number.

Prime means having exactly two positive divisors, `1` and the number
itself: the primes start `2, 3, 5, 7, 11`, and `1` is not one of them.

Count the valid layouts. The count soon gets huge, so return what is left
of it after dividing by `10^9 + 7` (that is, `1000000007`).

@examples

## Constraints

- `1 <= n <= 100`
