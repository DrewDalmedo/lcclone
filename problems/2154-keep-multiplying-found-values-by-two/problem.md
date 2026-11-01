id: 2154
slug: keep-multiplying-found-values-by-two
title: Keep Multiplying Found Values by Two
difficulty: easy
tags: array, hash-table, sorting, simulation
signature: int findFinalValue(int[] nums, int original)
---
Picture a ladder whose rungs are numbered `original`, `2 * original`,
`4 * original`, `8 * original`, and so on, each rung twice the one below
it. You start on the bottom rung, `original`. As long as the number of the
rung you stand on occurs somewhere in `nums`, you climb one rung up.

Return the number of the rung where you stop: the first rung, counting up
from `original`, whose number is not in `nums`. If `original` itself is
not in `nums`, you never climb, and the answer is `original`.

Only the rung numbers matter: where a number sits in `nums`, and how many
times it occurs there, makes no difference. Other numbers, such as
`3 * original` or `original / 2`, are not rungs and never help you climb.

@examples

## Constraints

- `1 <= nums.length <= 1000`
- `1 <= nums[i], original <= 1000`
