id: 1742
slug: maximum-number-of-balls-in-a-box
title: Maximum Number of Balls in a Box
difficulty: easy
tags: hash-table, math, counting
signature: int countBalls(int lowLimit, int highLimit)
---
There is one ball for every whole number from `lowLimit` to `highLimit`,
both included, with that number written on it. There are also boxes,
labelled `1`, `2`, `3` and so on, as many as needed. Each ball goes into
the box whose label is the total of the decimal digits on the ball, taken
in a single round of adding: ball `6098` goes into box `23`, not into box
`5`.

Once every ball is in its box, return how many balls the fullest box holds.
If several boxes tie for the most balls, that shared count is the answer.

@examples

## Constraints

- `1 <= lowLimit <= highLimit <= 10^5`
