id: 495
slug: teemo-attacking
title: Teemo Attacking
difficulty: easy
tags: array, simulation
signature: int findPoisonedDuration(int[] timeSeries, int duration)
---
`timeSeries` lists the seconds at which a target is struck with poison,
from earliest to latest; several strikes may land in the same second. A
strike at second `t` covers `duration` whole seconds, `t`, `t + 1`, ...,
`t + duration - 1`, and the target is poisoned in each of them. When
`duration` is `0`, a strike covers no second at all.

Return how many different seconds at least one strike covers. A second
that several strikes cover still counts only once. So a strike that
lands while the target is still poisoned adds no whole `duration` of its
own; it only carries the poisoning on to its own last second.

@examples

## Constraints

- `1 <= timeSeries.length <= 10^4`
- `0 <= timeSeries[i] <= 10^7`
- `timeSeries` is sorted in non-decreasing order.
- `0 <= duration <= 10^7`
