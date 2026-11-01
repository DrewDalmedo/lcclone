id: 3392
slug: count-subarrays-of-length-three-with-a-condition
title: Count Subarrays of Length Three With a Condition
difficulty: easy
tags: array
signature: int countSubarrays(int[] nums)
---
Every element of `nums` other than the first and the last stands between
two neighbours: the element just before it and the element just after it.
Call position `j`, with `0 < j < nums.length - 1`, a **pivot** when the
value there is exactly twice the sum of its two neighbours:

`nums[j] == 2 * (nums[j - 1] + nums[j + 1])`

Put the other way round, the two neighbours add up to exactly half of
`nums[j]`. The half is never rounded: an odd value such as `7` has half
`3.5`, which no sum of two integers equals, so an odd value is never a
pivot. Values may be negative or zero, and the same rule applies to them:
`-6` between `-1` and `-2` is a pivot, and so is `0` between `9` and `-9`.

Return the number of pivots in `nums`. Each position `j` is the middle of
exactly one subarray of length three, `[nums[j - 1], nums[j], nums[j + 1]]`,
so this is also the number of such subarrays whose middle element is twice
the sum of the other two. Pivots may sit side by side, and each one counts.

@examples

## Constraints

- `3 <= nums.length <= 100`
- `-100 <= nums[i] <= 100`
