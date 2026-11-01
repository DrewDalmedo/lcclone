id: 3178
slug: find-the-child-who-has-the-ball-after-k-seconds
title: Find the Child Who Has the Ball After K Seconds
difficulty: easy
tags: math, simulation
signature: int numberOfChild(int n, int k)
---
`n` children stand in a line, labelled `0, 1, ..., n - 1` from left to
right, and play catch. When the game starts, child `0` holds the ball.
Then, once every second, the child holding the ball throws it to a child
standing directly next to them, so the ball never skips anyone.

The ball travels in **legs**. A leg carries it from one end of the line to
the other, one child per second. The first leg runs rightwards, from child
`0` to child `n - 1`. Each leg after that starts where the previous one
ended and runs back the opposite way. There is no pause between legs: a
child at an end of the line throws the ball on during the second right
after catching it, like everyone else.

Return the label of the child who holds the ball once `k` seconds have
passed.

@examples

## Constraints

- `2 <= n <= 50`
- `1 <= k <= 50`
