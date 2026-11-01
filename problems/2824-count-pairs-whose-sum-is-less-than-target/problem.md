id: 2824
slug: count-pairs-whose-sum-is-less-than-target
title: Count Pairs Whose Sum is Less than Target
difficulty: easy
tags: array, two-pointers, binary-search, sorting
signature: int countPairs(int[] nums, int target)
---
Pick two different positions of `nums` and add up the values found there.
Count how many such picks give a total **strictly below** `target`, and
return that count.

A pick is a set of two positions: taking position `1` and then `4` is the
same pick as taking `4` and then `1`, so it is counted once, and no
position can be taken twice. Only positions matter, not values: if the
value `2` stands at three positions of `nums`, those three positions make
three different picks among themselves. A total exactly equal to `target`
is not counted.

@examples

## Constraints

- `1 <= nums.length <= 50`
- `-50 <= nums[i] <= 50`
- `-50 <= target <= 50`

## Follow-up

With `n` the length of `nums`, trying every pick takes `O(n^2)` time.
After sorting `nums`, can you count them in `O(n log n)`?
