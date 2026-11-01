id: 1512
slug: number-of-good-pairs
title: Number of Good Pairs
difficulty: easy
tags: array, hash-table, math, counting
signature: int numIdenticalPairs(int[] nums)
---
Think of `nums` as a row of tickets, one per index, where `nums[i]` is the
number printed on ticket `i`. Two different tickets form a **good pair**
when they show the same number.

A pair is just a set of two tickets: naming them in the other order gives
the same pair, and no ticket pairs with itself. Tickets that show the same
number still count separately, so three tickets showing `6` make three good
pairs.

Return how many good pairs the row holds.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
