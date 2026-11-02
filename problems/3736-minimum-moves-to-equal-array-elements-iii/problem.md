id: 3736
slug: minimum-moves-to-equal-array-elements-iii
title: Minimum Moves to Equal Array Elements III
difficulty: easy
tags: array, math
signature: int minMoves(int[] nums)
---
A row of towers is built from blocks: `nums[i]` is the height of tower
`i`, in blocks. In one move you put a single new block on top of one
tower of your choice. Blocks are never taken away, so a tower can only
grow.

Return the smallest number of moves after which all the towers have the
same height. If they already do, the answer is `0`.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
