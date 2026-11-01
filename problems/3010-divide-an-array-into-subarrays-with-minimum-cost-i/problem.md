id: 3010
slug: divide-an-array-into-subarrays-with-minimum-cost-i
title: Divide an Array Into Subarrays With Minimum Cost I
difficulty: easy
tags: array, enumeration, sorting
signature: int minimumCost(int[] nums)
---
Cut `nums` in two places, splitting it into three **pieces**. Each piece is
a run of one or more consecutive elements, and the pieces follow one
another with nothing left over: the first starts at `nums[0]`, each of the
others starts right after the one before it ends, and the third ends with
the last element.

Each piece has a **price**: the number it begins with. Whatever follows
that number inside the piece is free. Return the smallest total price of
the three pieces, over every way of making the two cuts.

@examples

## Constraints

- `3 <= nums.length <= 50`
- `1 <= nums[i] <= 50`
