id: 1854
slug: maximum-population-year
title: Maximum Population Year
difficulty: easy
tags: array, counting, prefix-sum
signature: int maximumPopulation(int[][] logs)
---
`logs` is a town register with one row per person. The row `[b, d]` says
the person was born in year `b` and died in year `d`: they lived through
every year from `b` up to `d - 1` (the years `b`, `b + 1`, ..., `d - 1`),
but not through year `d`.

For every year, count the people who lived through it; that count is the
year's **population**. Return the year with the largest population. When
several years share that largest population, return the one that comes
first.

@examples

## Constraints

- `1 <= logs.length <= 100`
- `logs[i].length == 2`
- `1950 <= logs[i][0] < logs[i][1] <= 2050`
