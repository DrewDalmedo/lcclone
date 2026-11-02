id: 3492
slug: maximum-containers-on-a-ship
title: Maximum Containers on a Ship
difficulty: easy
tags: math
signature: int maxContainers(int n, int w, int maxWeight)
---
A cargo ship's deck is marked out as a square grid with `n` rows of `n`
cells, and each cell has room for one container at most. All the
containers waiting at the dock are identical, each weighing `w`. The ship
can carry a total load of up to `maxWeight`: a load weighing exactly
`maxWeight` is still allowed, a heavier one is not.

Return the largest number of containers that can go on board without
running out of cells or going over the weight limit. It is `0` when a
single container already weighs more than `maxWeight`.

@examples

## Constraints

- `1 <= n <= 1000`
- `1 <= w <= 1000`
- `1 <= maxWeight <= 10^9`
