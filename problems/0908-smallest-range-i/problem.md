id: 908
slug: smallest-range-i
title: Smallest Range I
difficulty: easy
tags: array, math
signature: int smallestRangeI(int[] nums, int k)
---
The **spread** of an array is its largest element minus its smallest one.

Before the spread of `nums` is measured, each element may be shifted once:
an element `v` can become any integer from `v - k` to `v + k`, both
included. Every element makes its own choice, independently of the others,
and an element may also stay as it is.

Return the smallest spread `nums` can have after the shifts. Shifted values
are not held to the bounds below: they may drop under `0` or rise above
`10^4`.

@examples

## Constraints

- `1 <= nums.length <= 10^4`
- `0 <= nums[i] <= 10^4`
- `0 <= k <= 10^4`
