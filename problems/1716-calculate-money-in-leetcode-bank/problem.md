id: 1716
slug: calculate-money-in-leetcode-bank
title: Calculate Money in Leetcode Bank
difficulty: easy
tags: math
signature: int totalMoney(int n)
---
A savings account starts at zero, and once a day, for `n` days in a row, a
deposit is paid into it. The first day is a Monday, and the size of each
deposit follows a weekly pattern:

- in the first week the deposits climb by one a day: 1 on Monday, 2 on
  Tuesday, and so on up to 7 on Sunday;
- every later week climbs the same way, but starts one higher than the week
  before it: the second week pays 2, 3, ..., 8, the third pays 3, 4, ...,
  9, and so on.

So a Monday does not carry on from the Sunday just before it: it drops back
to one above what the Monday a week earlier paid. Put as a formula, if the
days are numbered from `0`, day `d` pays `d / 7 + d % 7 + 1`, where `d / 7`
is rounded down.

Nothing is ever taken out. Return the balance of the account at the end of
day `n`, that is, the total of the first `n` deposits.

@examples

## Constraints

- `1 <= n <= 1000`

## Follow-up

Adding the deposits up one day at a time is fast enough here. Can you find
the total with a fixed number of arithmetic steps, whatever `n` is?
