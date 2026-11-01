id: 2379
slug: minimum-recolors-to-get-k-consecutive-black-blocks
title: Minimum Recolors to Get K Consecutive Black Blocks
difficulty: easy
tags: string, sliding-window
signature: int minimumRecolors(string blocks, int k)
---
`blocks` describes a strip of cells, read from left to right: cell `i` is
black when `blocks[i]` is `'B'` and white when it is `'W'`. You would like
the strip to have `k` adjacent cells that are all black somewhere along it.

A **repaint** turns one white cell black, and each repaint counts as one.
Black cells are never turned white. Return the smallest number of repaints
after which some `k` adjacent cells are all black.

Only one group of `k` adjacent cells has to end up black; every other cell
may stay as it is. If such a group is black already, the answer is `0`.

@examples

## Constraints

- `1 <= k <= blocks.length <= 100`
- Each character of `blocks` is `'W'` or `'B'`.
