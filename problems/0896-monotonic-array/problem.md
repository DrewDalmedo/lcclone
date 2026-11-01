id: 896
slug: monotonic-array
title: Monotonic Array
difficulty: easy
tags: array
signature: bool isMonotonic(int[] nums)
---
Walk along `nums` from left to right, one step at a time: from `nums[0]` to
`nums[1]`, from `nums[1]` to `nums[2]`, and so on. A step **climbs** when
the value it reaches is larger than the one it leaves, **drops** when it is
smaller, and is **level** when the two are equal.

Return `false` if the walk has at least one climb and at least one drop,
wherever they are. Otherwise the values only ever move one way, or not at
all, so return `true`.

Level steps don't count either way, wherever they come. So an array of one
element, which has no steps, gives `true`, and so does an array whose values
are all equal.

@examples

## Constraints

- `1 <= nums.length <= 10^5`
- `-10^5 <= nums[i] <= 10^5`
