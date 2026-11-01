id: 1128
slug: number-of-equivalent-domino-pairs
title: Number of Equivalent Domino Pairs
difficulty: easy
tags: array, hash-table, counting
signature: int numEquivDominoPairs(int[][] dominoes)
---
Each entry of `dominoes` describes one domino tile by the two numbers
printed on its halves. A tile can be turned end for end, so `[2,7]` and
`[7,2]` describe tiles that look the same. Call two tiles **equivalent**
when their halves carry the same two numbers, in either order. A tile
whose halves are equal, such as `[4,4]`, is equivalent only to other
tiles with those same equal halves.

Count the ways to choose two different positions in `dominoes`, without
regard to their order, that hold equivalent tiles, and return that count.
Two tiles written the same way are equivalent too.

@examples

## Constraints

- `1 <= dominoes.length <= 4 * 10^4`
- `dominoes[i].length == 2`
- `1 <= dominoes[i][j] <= 9`
