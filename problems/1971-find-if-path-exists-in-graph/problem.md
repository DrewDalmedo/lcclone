id: 1971
slug: find-if-path-exists-in-graph
title: Find if Path Exists in Graph
difficulty: easy
tags: depth-first-search, breadth-first-search, union-find, graph
signature: bool validPath(int n, int[][] edges, int source, int destination)
---
A group of `n` islands, numbered from `0` to `n - 1`, is linked by
footbridges. Each entry `[u, v]` of `edges` is one footbridge between
islands `u` and `v`. It can be crossed both ways, whichever of the two
islands is written first.

You are standing on island `source`. Return `true` if you can get to island
`destination` by crossing footbridges one after another, as many as you
like, and `false` otherwise.

If `source` and `destination` are the same island, you are already there:
the answer is `true` even when that island has no footbridge at all.

@examples

## Constraints

- `1 <= n <= 2 * 10^5`
- `0 <= edges.length <= 2 * 10^5`
- `edges[i].length == 2`
- `0 <= edges[i][0], edges[i][1] <= n - 1`
- `edges[i][0] != edges[i][1]`: no footbridge leads from an island back to
  itself.
- Two islands are joined by at most one footbridge.
- `0 <= source, destination <= n - 1`
