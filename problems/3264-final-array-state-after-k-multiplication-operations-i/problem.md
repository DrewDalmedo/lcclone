id: 3264
slug: final-array-state-after-k-multiplication-operations-i
title: Final Array State After K Multiplication Operations I
difficulty: easy
tags: array, math, simulation, heap-priority-queue
signature: int[] getFinalState(int[] nums, int k, int multiplier)
---
A step multiplies one element of `nums` by `multiplier`: the element whose
value is the smallest in the array at that moment. When that smallest value
sits at more than one index, the leftmost of those elements is the one
multiplied. No other element changes in that step.

Do `k` steps, one after another. Each step sees the array as the earlier
steps left it, so an element that was just multiplied can still be the
smallest and be chosen again.

Return `nums` as it is after the `k`-th step, with every element still at
its own index.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`
- `1 <= k <= 10`
- `1 <= multiplier <= 5`
