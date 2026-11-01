id: 746
slug: min-cost-climbing-stairs
title: Min Cost Climbing Stairs
difficulty: easy
tags: array, dynamic-programming
signature: int minCostClimbingStairs(int[] cost)
---
A course is a line of launch pads, numbered `0` to `n - 1` where
`n = cost.length`, and after the last pad comes the finish. Pad `i`
charges `cost[i]` to launch you, and a launch sends you forward either to
the next pad or over it to the pad after that, as you choose. So a launch
from pad `n - 2` can reach the finish, and every launch from pad `n - 1`
does.

You may start on pad `0` or on pad `1`, and getting onto that first pad is
free. Return the smallest total of charges that gets you to the finish.

@examples

## Constraints

- `2 <= cost.length <= 1000`
- `0 <= cost[i] <= 999`
