id: 367
slug: valid-perfect-square
title: Valid Perfect Square
difficulty: easy
tags: math, binary-search
signature: bool isPerfectSquare(int num)
---
You have `num` square tiles, all the same size. Can you lay every one of
them out as a single solid square, with `k` rows of `k` tiles and none
left over? Return `true` if you can and `false` if you can't.

Put as arithmetic: return whether `num = k * k` for some whole number `k`,
that is, whether `num` is a **perfect square**. One tile alone already
makes a square, `1 = 1 * 1`.

The point is to find `k`, or rule it out, with integer arithmetic of your
own rather than by asking the math library, so leave `sqrt(num)` and
`pow(num, 0.5)` alone. Nothing checks how you got the answer: keeping to
this is up to you.

@examples

## Constraints

- `1 <= num <= 2^31 - 1`

## Follow-up

The squares `1, 4, 9, 16, ...` are the running sums of the odd numbers
`1, 3, 5, 7, ...`, which gives a method that takes `O(sqrt(num))` steps.
Is there a way that needs only `O(log num)` steps?
