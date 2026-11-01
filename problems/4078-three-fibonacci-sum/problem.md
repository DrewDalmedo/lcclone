id: 4078
slug: three-fibonacci-sum
title: Three Fibonacci Sum
difficulty: easy
tags:
signature: bool threeFibonacciSum(int n)
---
The Fibonacci sequence is given by `F(0) = 0`, `F(1) = 1` and
`F(k) = F(k - 1) + F(k - 2)` for every `k >= 2`. So the sequence begins
`0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ...`, and the value `1` appears twice in
it, as `F(1)` and as `F(2)`.

Decide whether the positive integer `n` is the total of three terms that
sit side by side in the sequence: return `true` if
`F(i) + F(i + 1) + F(i + 2) == n` for some index `i >= 0`, and `false`
otherwise.

Only terms at three neighbouring positions count. They may start at
`F(0)`, and the two `1`s are separate terms, so such a triple may hold one
or both of them.

@examples

## Constraints

- `1 <= n <= 10^9`
