id: 1217
slug: minimum-cost-to-move-chips-to-the-same-position
title: Minimum Cost to Move Chips to The Same Position
difficulty: easy
tags: greedy, array, math
signature: int minCostToMoveChips(int[] position)
---
Some chips lie on a number line, one entry of `position` per chip: chip `i`
sits at the whole-number coordinate `position[i]`. A coordinate that appears
several times in `position` holds that many separate chips, stacked.

You want all the chips to end up at one shared coordinate, which you are
free to pick; it need not hold a chip now. To get there you make moves, as
many as you like and in any order, and each move shifts a single chip:

- a jump of exactly 2 places, to the left or to the right, costs nothing;
- a step of exactly 1 place, to the left or to the right, costs 1.

A chip may land on, or pass over, coordinates where other chips lie. Return
the smallest total cost of bringing every chip to the same coordinate.

@examples

## Constraints

- `1 <= position.length <= 100`
- `1 <= position[i] <= 10^9`
