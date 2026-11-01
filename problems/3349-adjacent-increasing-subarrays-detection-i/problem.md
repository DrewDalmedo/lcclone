id: 3349
slug: adjacent-increasing-subarrays-detection-i
title: Adjacent Increasing Subarrays Detection I
difficulty: easy
tags: array
signature: bool hasIncreasingSubarrays(int[] nums, int k)
---
Take a stretch of `2 * k` consecutive elements of `nums` and cut it down
the middle into a front half and a back half of `k` elements each. The cut
**works** when each half is strictly increasing on its own: reading the
half from left to right, every value is larger than the previous one.

Nothing is required where the halves meet. The last element of the front
half may be smaller than, equal to or greater than the first element of the
back half.

Return `true` if some stretch of `nums` can be cut so that it works, and
`false` otherwise. A half of a single element counts as strictly
increasing, so with `k = 1` the answer is `true`.

@examples

## Constraints

- `2 <= nums.length <= 100`
- `1 < 2 * k <= nums.length`
- `-1000 <= nums[i] <= 1000`
