id: 3925
slug: concatenate-array-with-reverse
title: Concatenate Array With Reverse
difficulty: easy
tags: array, simulation
signature: int[] concatWithReverse(int[] nums)
---
Stand a mirror just past the last value of `nums`. Looking from the front,
you see the `n` values themselves and then, beyond them, their reflection:
the same values in the opposite order, starting with the one closest to the
mirror.

Return what you see as a new array of `2n` integers: first `nums[0]`,
`nums[1]`, ..., `nums[n - 1]`, and then `nums[n - 1]`, `nums[n - 2]`, ...,
`nums[0]`. So `nums[n - 1]` appears twice side by side in the middle,
`nums[0]` both opens and closes the array, and the array reads the same
from either end. In index terms, `nums[i]` goes to position `i` and to
position `2n - 1 - i`, for every `i` from `0` to `n - 1`.

@examples

## Constraints

- `n == nums.length`
- `1 <= n <= 100`
- `1 <= nums[i] <= 100`
