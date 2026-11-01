id: 1791
slug: find-center-of-star-graph
title: Find Center of Star Graph
difficulty: easy
tags: graph
signature: int findCenter(int[][] edges)
---
`edges` describes an undirected graph whose nodes are numbered `1` to `n`,
where `n = edges.length + 1`. Each `edges[i] = [u, v]` is one edge, joining
nodes `u` and `v`.

The graph is known to be a **star**: one node, its **center**, has an edge
to every other node, and no edge leaves out the center. So each of the
other nodes lies on exactly one edge, the one that ties it to the center.
Return the number of the center.

The edges may come in any order, and in each pair the center may be
written first or second.

@examples

## Constraints

- `3 <= n <= 10^5`
- `edges.length == n - 1`
- `edges[i].length == 2`
- `1 <= edges[i][0], edges[i][1] <= n`
- `edges[i][0] != edges[i][1]`
- `edges` forms a star on the nodes `1` to `n`.
