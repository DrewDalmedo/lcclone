id: 3861
slug: minimum-capacity-box
title: Minimum Capacity Box
difficulty: easy
tags: array
signature: int minimumIndex(int[] capacity, int itemSize)
---
You have a single item of size `itemSize` to pack. The boxes on hand are
numbered from `0`, and box `i` can hold anything whose size is at most
`capacity[i]`. So the item **fits** in box `i` exactly when
`capacity[i] >= itemSize`; a box of the same size as the item will do.

Pack it as snugly as you can: among the boxes the item fits in, pick one
with the smallest capacity. If several of them share that smallest
capacity, pick the one with the lowest number. Return the number of the
box you picked (its index in `capacity`, not its capacity).

When every box is smaller than the item, the answer is `-1`.

@examples

## Constraints

- `1 <= capacity.length <= 100`
- `1 <= capacity[i] <= 100`
- `1 <= itemSize <= 100`
