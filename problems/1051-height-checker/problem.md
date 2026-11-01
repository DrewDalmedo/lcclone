id: 1051
slug: height-checker
title: Height Checker
difficulty: easy
tags: array, counting-sort, sorting
signature: int heightChecker(int[] heights)
---
Seedlings in pots stand in a row along a windowsill. Counting the pots from
the left, starting at `0`, the seedling in pot `i` is `heights[i]`
centimetres tall.

In a **tidy** row the same seedlings would go from shortest on the left to
tallest on the right, with seedlings of equal height next to each other.
Walk along the actual row and the tidy row together, pot by pot, and return
the number of pots where the two heights differ.

Only heights are compared, so two seedlings of the same height are
interchangeable. The answer is that number of pots, not the number of
moves or swaps it would take to tidy the row.

@examples

## Constraints

- `1 <= heights.length <= 100`
- `1 <= heights[i] <= 100`
