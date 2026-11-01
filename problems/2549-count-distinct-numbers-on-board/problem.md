id: 2549
slug: count-distinct-numbers-on-board
title: Count Distinct Numbers on Board
difficulty: easy
tags: array, hash-table, math, simulation
signature: int distinctIntegers(int n)
---
The houses of a village are numbered from `1` to `n`. At first only house
`n` has heard a piece of news. Every day, each house `x` that knows the
news passes it on to every house `i` with `1 <= i <= n` such that dividing
`x` by `i` leaves a remainder of exactly `1` (that is, `x mod i = 1`). A
house that has heard the news never forgets it, and hearing it again
changes nothing.

This goes on for `10^9` days. Return how many houses know the news at the
end, counting house `n` itself.

@examples

## Constraints

- `1 <= n <= 100`
