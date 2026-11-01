id: 509
slug: fibonacci-number
title: Fibonacci Number
difficulty: easy
tags: recursion, memoization, math, dynamic-programming
signature: int fib(int n)
---
Write down `0`, then `1`. From there on, keep extending the list by
appending the sum of its last two numbers. The numbers that turn up this
way, `0, 1, 1, 2, 3, 5, 8` and onwards, are the **Fibonacci numbers**.

Number the positions in the list from `0`, so that position `0` holds `0`
and position `1` holds `1`. Return the number at position `n`.

@examples

## Constraints

- `0 <= n <= 30`

## Follow-up

Extending the list one number at a time takes `n` steps. Is there a way
that takes only `O(log n)` steps?
