id: 2970
slug: count-the-number-of-incremovable-subarrays-i
title: Count the Number of Incremovable Subarrays I
difficulty: easy
tags: array, two-pointers, binary-search, enumeration
signature: int incremovableSubarrayCount(int[] nums)
---
Let `n` be the length of `nums`. A **stretch** of `nums` is one or more
neighbouring elements, from position `l` to position `r`, where
`0 <= l <= r <= n - 1`. Cutting a stretch out leaves the elements before
position `l` followed directly by the elements after position `r`, still in
their original order. Either of those two parts may be empty.

A stretch is **incremovable** when the elements left after cutting it out
rise strictly from left to right: each one is less than the one right after
it, so a pair of equal values side by side is not allowed. This applies
across the cut too: `nums[l - 1]` and `nums[r + 1]` (when both exist) end
up side by side once the stretch between them is gone. When nothing is
left, or a single element, it counts as rising.

Return how many stretches are incremovable. Stretches are told apart by
their positions `l` and `r`, not by the values they hold, so two stretches
with the same numbers in different places are counted separately. Cutting
out nothing at all is not a choice: every stretch has at least one element.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 50`
