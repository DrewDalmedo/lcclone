id: 2303
slug: calculate-amount-paid-in-taxes
title: Calculate Amount Paid in Taxes
difficulty: easy
tags: array, simulation
signature: double calculateTax(int[][] brackets, int income)
---
Income is taxed in brackets. `brackets[i] = [upper_i, percent_i]`, and the
upper bounds strictly increase from one bracket to the next. Bracket `0`
holds the dollars of an income from `0` up to `upper_0`; bracket `i`, for
`i > 0`, holds the dollars above `upper_(i-1)` up to and including
`upper_i`. Every dollar in bracket `i` is taxed at `percent_i` percent.

So bracket `i` takes `percent_i / 100` times the number of dollars of the
income that fall in it, which is `min(income, upper_i) - upper_(i-1)` when
that is positive and `0` otherwise (with `upper_(-1) = 0`). A bracket that
starts at or above `income` takes nothing.

Return the total tax on an income of `income` dollars. An answer within
`10^-5` of the exact amount is accepted.

@examples

## Constraints

- `1 <= brackets.length <= 100`
- `brackets[i].length == 2`
- `1 <= upper_i <= 1000`
- `0 <= percent_i <= 100`
- `0 <= income <= 1000`
- `upper_i` is strictly increasing in `i`.
- The last upper bound is at least `income`.
