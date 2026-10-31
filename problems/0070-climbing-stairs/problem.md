id: 70
slug: climbing-stairs
title: Climbing Stairs
difficulty: easy
tags: math, dynamic-programming, memoization
signature: int climbStairs(int n)
---
A staircase has `n` steps. You start at the bottom, and with each move you
go up either **one** step or **two** steps. Return the number of different
sequences of moves that take you exactly to the top.

Two sequences are different when they differ in at least one move, even if
they use the same number of one-step and two-step moves.

@examples

## Constraints

- `1 <= n <= 45`

## Follow-up

How many moves does the last move of a sequence leave before it? Use that to
build the answer for `n` from smaller answers, keeping only two numbers at a
time.
