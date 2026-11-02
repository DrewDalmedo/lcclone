id: 3502
slug: minimum-cost-to-reach-every-position
title: Minimum Cost to Reach Every Position
difficulty: easy
tags: array
signature: int[] minCosts(int[] cost)
---
A queue has `n + 1` places, numbered `0` at the front to `n` at the back.
You stand at place `n`. Each of the places `0` to `n - 1` holds one
person, and the person who starts at place `i` charges a fee of
`cost[i]`. A fee belongs to its person and moves with them.

You move only by trading places with another person in the queue. You may
trade with anyone, not just a neighbour, as many times as you like:

- trading with someone **ahead** of you (at a smaller place number) costs
  you that person's fee;
- trading with someone **behind** you (at a larger place number) is free.

For each place `i` from `0` to `n - 1`, find the least total you must pay
to end up standing at place `i`, each time starting from the original
queue. Return an array `answer` of length `n` in which `answer[i]` is
that amount for place `i`.

@examples

## Constraints

- `1 <= n == cost.length <= 100`
- `1 <= cost[i] <= 100`
